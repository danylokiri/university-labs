// Лабораторна 5. Паралельний метод Гауса для розв'язання СЛАУ A·x = b (MPI).
// Розподіл рядків матриці — циклічний (round-robin) між процесами:
// процес p отримує рядки p, p+np, p+2np, …. Це забезпечує балансування
// навантаження у прямому ході (у пізніх ітераціях активних рядків
// залишається небагато, і циклічний розподіл тримає всі процеси
// зайнятими довше, ніж блоковий).
//
// Прямий хід: на ітерації i власник рядка i (rank = i mod np) поширює
// цей рядок усім процесам через MPI_Bcast, після чого кожен процес
// віднімає масштабовані копії рядка i від своїх локальних рядків з
// глобальними індексами gid > i.
//
// Зворотний хід: починаючи з i = N−1, власник рядка i обчислює x_i
// як (b_i − Σ a_ij · x_j) / a_ii, поширює через MPI_Bcast, далі всі
// процеси оновлюють відповідні b у своїх локальних рядках. Остаточне
// розв'язання повертається майстром.

#include <mpi.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <random>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int N = (argc > 1) ? std::atoi(argv[1]) : 512;

    // Майстер генерує систему A·x = b та рахує послідовний час
    std::vector<double> A0, b0;
    std::vector<double> xseq;
    double tseq = 0.0;
    if (rank == 0) {
        A0.assign(N*N, 0.0); b0.assign(N, 0.0);
        std::mt19937_64 rng(15);
        std::uniform_real_distribution<double> d(-10.0, 10.0);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) A0[i*N+j] = d(rng);
            A0[i*N+i] += 50.0;          // діагональне домінування
            b0[i] = d(rng);
        }
        // Послідовний Гаус
        auto A = A0; auto b = b0;
        double t0 = MPI_Wtime();
        for (int i = 0; i < N-1; ++i) {
            double piv = A[i*N + i];
            for (int r = i+1; r < N; ++r) {
                double f = A[r*N + i] / piv;
                for (int j = i; j < N; ++j) A[r*N + j] -= f * A[i*N + j];
                b[r] -= f * b[i];
            }
        }
        xseq.assign(N, 0.0);
        for (int i = N-1; i >= 0; --i) {
            double s = b[i];
            for (int j = i+1; j < N; ++j) s -= A[i*N + j] * xseq[j];
            xseq[i] = s / A[i*N + i];
        }
        tseq = MPI_Wtime() - t0;
    }

    // Розсилаємо N всім (хоча вже відомо)
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Визначаємо, які глобальні номери рядків припадають на цей процес
    std::vector<int> gids;
    for (int i = rank; i < N; i += np) gids.push_back(i);
    int my_rows = (int)gids.size();
    std::vector<double> A_loc(my_rows * N, 0.0);
    std::vector<double> b_loc(my_rows, 0.0);

    // Розсилаємо рядки блокуючим Send з майстра
    if (rank == 0) {
        for (int p = 0; p < np; ++p) {
            std::vector<int> pg;
            for (int i = p; i < N; i += np) pg.push_back(i);
            if (p == 0) {
                for (size_t k = 0; k < pg.size(); ++k) {
                    std::memcpy(&A_loc[k*N], &A0[pg[k]*N], N*sizeof(double));
                    b_loc[k] = b0[pg[k]];
                }
            } else {
                int cnt = (int)pg.size();
                std::vector<double> bufA((size_t)cnt*N);
                std::vector<double> bufB(cnt);
                for (int k = 0; k < cnt; ++k) {
                    std::memcpy(&bufA[(size_t)k*N], &A0[pg[k]*N],
                                N*sizeof(double));
                    bufB[k] = b0[pg[k]];
                }
                MPI_Send(&cnt, 1, MPI_INT, p, 0, MPI_COMM_WORLD);
                MPI_Send(bufA.data(), cnt*N, MPI_DOUBLE, p, 1, MPI_COMM_WORLD);
                MPI_Send(bufB.data(), cnt,   MPI_DOUBLE, p, 2, MPI_COMM_WORLD);
            }
        }
    } else {
        int cnt;
        MPI_Recv(&cnt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // cnt має збігатися з my_rows
        MPI_Recv(A_loc.data(), cnt*N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b_loc.data(), cnt,   MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // ---- Прямий хід ----
    std::vector<double> pivot_row(N + 1);
    for (int i = 0; i < N-1; ++i) {
        int owner = i % np;
        if (rank == owner) {
            int local_idx = i / np;
            std::memcpy(pivot_row.data(), &A_loc[(size_t)local_idx*N],
                        N*sizeof(double));
            pivot_row[N] = b_loc[local_idx];
        }
        MPI_Bcast(pivot_row.data(), N+1, MPI_DOUBLE, owner, MPI_COMM_WORLD);
        double piv = pivot_row[i];
        for (int r = 0; r < my_rows; ++r) {
            int gid = gids[r];
            if (gid <= i) continue;
            double factor = A_loc[(size_t)r*N + i] / piv;
            for (int j = i; j < N; ++j)
                A_loc[(size_t)r*N + j] -= factor * pivot_row[j];
            b_loc[r] -= factor * pivot_row[N];
        }
    }

    // ---- Зворотний хід ----
    std::vector<double> x(N, 0.0);
    for (int i = N-1; i >= 0; --i) {
        int owner = i % np;
        double xi = 0.0;
        if (rank == owner) {
            int local_idx = i / np;
            double s = b_loc[local_idx];
            for (int j = i+1; j < N; ++j)
                s -= A_loc[(size_t)local_idx*N + j] * x[j];
            xi = s / A_loc[(size_t)local_idx*N + i];
        }
        MPI_Bcast(&xi, 1, MPI_DOUBLE, owner, MPI_COMM_WORLD);
        x[i] = xi;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double tpar = MPI_Wtime() - t0;

    // Обчислюємо residual ‖A·x − b‖∞ і max|x − x_seq| на майстрі
    if (rank == 0) {
        double resmax = 0.0;
        for (int i = 0; i < N; ++i) {
            double s = -b0[i];
            for (int j = 0; j < N; ++j) s += A0[i*N + j] * x[j];
            if (std::abs(s) > resmax) resmax = std::abs(s);
        }
        double diffmax = 0.0;
        for (int i = 0; i < N; ++i)
            if (std::abs(x[i] - xseq[i]) > diffmax)
                diffmax = std::abs(x[i] - xseq[i]);
        double S = tseq / tpar, E = S / np;
        std::printf("%d,%d,%.6f,%.6f,%.4f,%.4f,%.3e,%.3e\n",
                    N, np, tseq, tpar, S, E, resmax, diffmax);
    }
    MPI_Finalize();
    return 0;
}
