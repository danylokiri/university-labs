// Лабораторна 4. Блоковий алгоритм Кенона (Cannon) матричного множення
// C = A·B (MPI). Вимагає p = q², N ділиться на q.
// Процеси — тороїдальна решітка q×q.
// Етап ініціалізації: блоки A зсуваються ліворуч на i позицій у рядку i,
// блоки B зсуваються вгору на j позицій у стовпці j.
// Основний цикл: q ітерацій — локальне множення блоків, після чого
// одинарний циклічний зсув A ліворуч і B вгору.
// Відмінність від Фокса: початковий зсув замість MPI_Bcast у рядку;
// на кожній ітерації — одинарні зсуви замість трансляцій.

#include <mpi.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <cmath>

static void dgemm_block(const double* A, const double* B, double* C,
                        int blk) {
    for (int i = 0; i < blk; ++i)
        for (int k = 0; k < blk; ++k) {
            double aik = A[i*blk + k];
            for (int j = 0; j < blk; ++j)
                C[i*blk + j] += aik * B[k*blk + j];
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
            std::printf("cannon,%d,%d,SKIP_p_not_square_or_N_not_div_q\n", N, np);
        MPI_Finalize(); return 0;
    }
    int blk = N / q;

    MPI_Comm grid;
    int dims[2] = {q, q}, periods[2] = {1, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid);
    int coords[2];
    MPI_Cart_coords(grid, rank, 2, coords);
    int row = coords[0], col = coords[1];

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

    // Розсилання блоків A_ij, B_ij
    std::vector<double> Ablk(blk*blk), Bblk(blk*blk);
    if (rank == 0) {
        for (int pr = 0; pr < q; ++pr)
            for (int pc = 0; pc < q; ++pc) {
                int dcoords[2] = {pr, pc};
                int dest;
                MPI_Cart_rank(grid, dcoords, &dest);
                if (dest == 0) {
                    for (int i = 0; i < blk; ++i) {
                        std::memcpy(&Ablk[i*blk], &A[(pr*blk + i)*N + pc*blk],
                                    blk*sizeof(double));
                        std::memcpy(&Bblk[i*blk], &B[(pr*blk + i)*N + pc*blk],
                                    blk*sizeof(double));
                    }
                } else {
                    // пакуємо вручну і надсилаємо
                    std::vector<double> bufA(blk*blk), bufB(blk*blk);
                    for (int i = 0; i < blk; ++i) {
                        std::memcpy(&bufA[i*blk], &A[(pr*blk + i)*N + pc*blk],
                                    blk*sizeof(double));
                        std::memcpy(&bufB[i*blk], &B[(pr*blk + i)*N + pc*blk],
                                    blk*sizeof(double));
                    }
                    MPI_Send(bufA.data(), blk*blk, MPI_DOUBLE, dest, 100, grid);
                    MPI_Send(bufB.data(), blk*blk, MPI_DOUBLE, dest, 101, grid);
                }
            }
    } else {
        MPI_Recv(Ablk.data(), blk*blk, MPI_DOUBLE, 0, 100, grid, MPI_STATUS_IGNORE);
        MPI_Recv(Bblk.data(), blk*blk, MPI_DOUBLE, 0, 101, grid, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(grid);
    double t0 = MPI_Wtime();

    // Початковий зсув: A ліворуч на row позицій, B вгору на col позицій
    int src, dst;
    MPI_Cart_shift(grid, 1, -row, &src, &dst);
    MPI_Sendrecv_replace(Ablk.data(), blk*blk, MPI_DOUBLE,
                         dst, 0, src, 0, grid, MPI_STATUS_IGNORE);
    MPI_Cart_shift(grid, 0, -col, &src, &dst);
    MPI_Sendrecv_replace(Bblk.data(), blk*blk, MPI_DOUBLE,
                         dst, 1, src, 1, grid, MPI_STATUS_IGNORE);

    std::vector<double> Cblk(blk*blk, 0.0);
    for (int l = 0; l < q; ++l) {
        dgemm_block(Ablk.data(), Bblk.data(), Cblk.data(), blk);
        // одинарний зсув A ліворуч, B вгору
        MPI_Cart_shift(grid, 1, -1, &src, &dst);
        MPI_Sendrecv_replace(Ablk.data(), blk*blk, MPI_DOUBLE,
                             dst, 2, src, 2, grid, MPI_STATUS_IGNORE);
        MPI_Cart_shift(grid, 0, -1, &src, &dst);
        MPI_Sendrecv_replace(Bblk.data(), blk*blk, MPI_DOUBLE,
                             dst, 3, src, 3, grid, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(grid);
    double tpar = MPI_Wtime() - t0;

    // Збираємо результат
    std::vector<double> Call;
    if (rank == 0) Call.assign(N*N, 0.0);
    if (rank == 0) {
        for (int i = 0; i < blk; ++i)
            std::memcpy(&Call[i*N + 0], &Cblk[i*blk], blk*sizeof(double));
        for (int pr = 0; pr < q; ++pr)
            for (int pc = 0; pc < q; ++pc) {
                int dcoords[2] = {pr, pc};
                int src_r;
                MPI_Cart_rank(grid, dcoords, &src_r);
                if (src_r == 0) continue;
                std::vector<double> tmp(blk*blk);
                MPI_Recv(tmp.data(), blk*blk, MPI_DOUBLE, src_r, 200,
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
        std::printf("cannon,%d,%d,%.6f,%.6f,%.4f,%.4f,%.3e\n",
                    N, np, tseq, tpar, S, E, maxdiff);
    }
    MPI_Comm_free(&grid);
    MPI_Finalize();
    return 0;
}
