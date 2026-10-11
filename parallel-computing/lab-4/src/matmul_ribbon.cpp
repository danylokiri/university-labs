// Лабораторна 4. Стрічковий алгоритм матричного множення C = A·B (MPI).
// Розподіл A — по рядках (смуги n/p рядків), B — цілком копіюється
// на кожен процес через MPI_Bcast.
// Збір результату — MPI_Gatherv.
// Блокуючі колективні операції.

#include <mpi.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <cmath>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int N = (argc > 1) ? std::atoi(argv[1]) : 256;

    std::vector<double> A, B(N*N), C, Cseq;
    double tseq = 0.0;

    // Майстер генерує A, B; обчислює послідовний еталон.
    if (rank == 0) {
        A.assign(N*N, 0.0); C.assign(N*N, 0.0); Cseq.assign(N*N, 0.0);
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

    // Розсилаємо B (цілком) усім процесам
    MPI_Bcast(B.data(), N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Розсилаємо смуги A через Scatterv
    int base = N / np, rem = N % np;
    std::vector<int> rcounts(np), rdispls(np);
    std::vector<int> rowcount(np);
    int cum = 0;
    for (int p = 0; p < np; ++p) {
        rowcount[p] = base + (p < rem ? 1 : 0);
        rcounts[p] = rowcount[p] * N;
        rdispls[p] = cum;
        cum += rcounts[p];
    }
    std::vector<double> Aloc(rcounts[rank]);
    MPI_Scatterv(rank == 0 ? A.data() : nullptr,
                 rcounts.data(), rdispls.data(), MPI_DOUBLE,
                 Aloc.data(), rcounts[rank], MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    int my_rows = rowcount[rank];
    std::vector<double> Cloc(my_rows * N, 0.0);
    for (int i = 0; i < my_rows; ++i)
        for (int k = 0; k < N; ++k) {
            double aik = Aloc[i*N + k];
            for (int j = 0; j < N; ++j)
                Cloc[i*N + j] += aik * B[k*N + j];
        }
    MPI_Barrier(MPI_COMM_WORLD);
    double tpar = MPI_Wtime() - t0;

    // Збираємо C через Gatherv
    MPI_Gatherv(Cloc.data(), my_rows*N, MPI_DOUBLE,
                rank == 0 ? C.data() : nullptr,
                rcounts.data(), rdispls.data(), MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        double maxdiff = 0.0;
        for (int i = 0; i < N*N; ++i) {
            double dd = std::abs(C[i] - Cseq[i]);
            if (dd > maxdiff) maxdiff = dd;
        }
        double S = tseq / tpar, E = S / np;
        std::printf("ribbon,%d,%d,%.6f,%.6f,%.4f,%.4f,%.3e\n",
                    N, np, tseq, tpar, S, E, maxdiff);
    }
    MPI_Finalize();
    return 0;
}
