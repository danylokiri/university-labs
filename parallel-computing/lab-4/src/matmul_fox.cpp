// Лабораторна 4. Блоковий алгоритм Фокса матричного множення C = A·B (MPI).
// Вимагає p = q², N ділиться на q.
// Процеси утворюють двовимірну тороїдальну декартову решітку q×q.
// На кожній з q ітерацій l: у рядку i процес із колонкою (i+l) mod q
// транслює свій блок A через MPI_Bcast в row-комунікаторі, виконується
// C_ij += A'·B_ij, після чого блоки B зсуваються на один крок угору
// в column-напрямку через MPI_Sendrecv_replace.

#include <mpi.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <cmath>

static void dgemm_block(const double* A, const double* B, double* C,
                        int blk, int ldA, int ldB, int ldC) {
    for (int i = 0; i < blk; ++i)
        for (int k = 0; k < blk; ++k) {
            double aik = A[i*ldA + k];
            for (int j = 0; j < blk; ++j)
                C[i*ldC + j] += aik * B[k*ldB + j];
        }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int N = (argc > 1) ? std::atoi(argv[1]) : 256;

    int q = (int)std::sqrt((double)np);
    if (q*q != np || N % q != 0) {
        if (rank == 0)
            std::printf("fox,%d,%d,SKIP_p_not_square_or_N_not_div_q\n", N, np);
        MPI_Finalize(); return 0;
    }
    int blk = N / q;

    // двовимірна периодична декартова решітка
    MPI_Comm grid;
    int dims[2] = {q, q}, periods[2] = {1, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid);
    int coords[2];
    MPI_Cart_coords(grid, rank, 2, coords);
    int row = coords[0], col = coords[1];

    MPI_Comm row_comm, col_comm;
    int row_keep[2] = {0, 1}, col_keep[2] = {1, 0};
    MPI_Cart_sub(grid, row_keep, &row_comm);
    MPI_Cart_sub(grid, col_keep, &col_comm);

    // Майстер готує A, B та послідовний еталон
    std::vector<double> A, B, Cseq;
    double tseq = 0.0;
    if (rank == 0) {
        A.assign(N*N, 0.0); B.assign(N*N, 0.0); Cseq.assign(N*N, 0.0);
        std::mt19937_64 rng(15);
        std::uniform_real_distribution<double> d(-1.0, 1.0);
        for (int i = 0; i < N*N; ++i) { A[i] = d(rng); B[i] = d(rng); }
        double t0 = MPI_Wtime();
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k) {
                double aik = A[i*N+k];
                for (int j = 0; j < N; ++j)
                    Cseq[i*N+j] += aik * B[k*N+j];
            }
        tseq = MPI_Wtime() - t0;
    }

    // Розсилання блоків A_ij та B_ij по процесах решітки.
    // Формуємо MPI_Type_vector для блоку blk×blk із кроком N.
    MPI_Datatype blk_t;
    MPI_Type_vector(blk, blk, N, MPI_DOUBLE, &blk_t);
    MPI_Type_create_resized(blk_t, 0, blk * sizeof(double), &blk_t);
    MPI_Type_commit(&blk_t);

    std::vector<double> Ablk(blk*blk), Bblk(blk*blk);
    // Майстер посилає кожному свої блоки через Send; кожен процес отримує Recv.
    if (rank == 0) {
        for (int pr = 0; pr < q; ++pr)
            for (int pc = 0; pc < q; ++pc) {
                int dest;
                int dcoords[2] = {pr, pc};
                MPI_Cart_rank(grid, dcoords, &dest);
                int idx = (pr*N + pc) * blk;
                if (dest == 0) {
                    // копіюємо у свій локальний блок
                    for (int i = 0; i < blk; ++i) {
                        std::memcpy(&Ablk[i*blk], &A[(pr*blk + i)*N + pc*blk],
                                    blk*sizeof(double));
                        std::memcpy(&Bblk[i*blk], &B[(pr*blk + i)*N + pc*blk],
                                    blk*sizeof(double));
                    }
                } else {
                    MPI_Send(&A[idx], 1, blk_t, dest, 100, grid);
                    MPI_Send(&B[idx], 1, blk_t, dest, 101, grid);
                }
            }
    } else {
        MPI_Recv(Ablk.data(), blk*blk, MPI_DOUBLE, 0, 100, grid, MPI_STATUS_IGNORE);
        MPI_Recv(Bblk.data(), blk*blk, MPI_DOUBLE, 0, 101, grid, MPI_STATUS_IGNORE);
    }

    std::vector<double> Cblk(blk*blk, 0.0);
    std::vector<double> Abuf(blk*blk);

    MPI_Barrier(grid);
    double t0 = MPI_Wtime();
    for (int l = 0; l < q; ++l) {
        int root = (row + l) % q;
        if (col == root) std::memcpy(Abuf.data(), Ablk.data(), blk*blk*sizeof(double));
        MPI_Bcast(Abuf.data(), blk*blk, MPI_DOUBLE, root, row_comm);
        dgemm_block(Abuf.data(), Bblk.data(), Cblk.data(), blk, blk, blk, blk);
        // зсув B на одну позицію вгору по стовпцю
        int src, dst;
        MPI_Cart_shift(grid, 0, -1, &src, &dst);
        MPI_Sendrecv_replace(Bblk.data(), blk*blk, MPI_DOUBLE,
                             dst, 7, src, 7, grid, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(grid);
    double tpar = MPI_Wtime() - t0;

    // Збираємо результат Cblk на майстрі
    std::vector<double> Call;
    if (rank == 0) Call.assign(N*N, 0.0);
    if (rank == 0) {
        // копіюємо свій блок
        for (int i = 0; i < blk; ++i)
            std::memcpy(&Call[i*N + 0], &Cblk[i*blk], blk*sizeof(double));
        for (int pr = 0; pr < q; ++pr)
            for (int pc = 0; pc < q; ++pc) {
                int dcoords[2] = {pr, pc};
                int src;
                MPI_Cart_rank(grid, dcoords, &src);
                if (src == 0) continue;
                std::vector<double> tmp(blk*blk);
                MPI_Recv(tmp.data(), blk*blk, MPI_DOUBLE, src, 200,
                         grid, MPI_STATUS_IGNORE);
                for (int i = 0; i < blk; ++i)
                    std::memcpy(&Call[(pr*blk + i)*N + pc*blk],
                                &tmp[i*blk], blk*sizeof(double));
            }
    } else {
        MPI_Send(Cblk.data(), blk*blk, MPI_DOUBLE, 0, 200, grid);
    }

    if (rank == 0) {
        double maxdiff = 0.0;
        for (int i = 0; i < N*N; ++i) {
            double dd = std::abs(Call[i] - Cseq[i]);
            if (dd > maxdiff) maxdiff = dd;
        }
        double S = tseq / tpar, E = S / np;
        std::printf("fox,%d,%d,%.6f,%.6f,%.4f,%.4f,%.3e\n",
                    N, np, tseq, tpar, S, E, maxdiff);
    }
    MPI_Type_free(&blk_t);
    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&grid);
    MPI_Finalize();
    return 0;
}
