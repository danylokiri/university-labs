// Лабораторна 2, варіант 15, задача 4.
// Знаходження середнього значення для заданого набору числових даних.
// Реалізація: блокуючий двоточковий обмін MPI_Send / MPI_Recv.
// Майстер розподіляє смуги даних; кожен процес обчислює локальну суму;
// часткові суми збираються на майстрі, що ділить підсумкову суму на N.

#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long N = (argc > 1) ? std::atol(argv[1]) : 10000000;
    int repeats = (argc > 2) ? std::atoi(argv[2]) : 5;

    std::vector<double> data;
    if (rank == 0) {
        data.resize(N);
        std::mt19937_64 rng(15);
        std::uniform_real_distribution<double> d(-100.0, 100.0);
        for (long i = 0; i < N; ++i) data[i] = d(rng);
    }

    double seq_time = 0.0, seq_avg = 0.0;
    if (rank == 0) {
        double tmin = 1e30;
        for (int r = 0; r < repeats; ++r) {
            double t0 = MPI_Wtime();
            double s = 0.0;
            for (long i = 0; i < N; ++i) s += data[i];
            double t1 = MPI_Wtime();
            seq_avg = s / (double)N;
            if (t1 - t0 < tmin) tmin = t1 - t0;
        }
        seq_time = tmin;
    }

    long base = N / size;
    long rem  = N % size;

    double par_time = 0.0, par_avg = 0.0;
    double tmin_par = 1e30;
    for (int r = 0; r < repeats; ++r) {
        MPI_Barrier(MPI_COMM_WORLD);
        double t0 = MPI_Wtime();

        std::vector<double> myD;
        long my_n = 0;
        if (rank == 0) {
            my_n = base + (rem > 0 ? 1 : 0);
            myD.assign(data.begin(), data.begin() + my_n);
            long offset = my_n;
            for (int p = 1; p < size; ++p) {
                long cnt = base + (p < rem ? 1 : 0);
                MPI_Send(&cnt, 1, MPI_LONG, p, 100, MPI_COMM_WORLD);
                MPI_Send(data.data() + offset, (int)cnt, MPI_DOUBLE, p, 101, MPI_COMM_WORLD);
                offset += cnt;
            }
        } else {
            MPI_Status st;
            MPI_Recv(&my_n, 1, MPI_LONG, 0, 100, MPI_COMM_WORLD, &st);
            myD.resize(my_n);
            MPI_Recv(myD.data(), (int)my_n, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD, &st);
        }

        double local = 0.0;
        for (long i = 0; i < my_n; ++i) local += myD[i];

        if (rank == 0) {
            double total = local;
            for (int p = 1; p < size; ++p) {
                double partial;
                MPI_Status st;
                MPI_Recv(&partial, 1, MPI_DOUBLE, p, 200, MPI_COMM_WORLD, &st);
                total += partial;
            }
            par_avg = total / (double)N;
        } else {
            MPI_Send(&local, 1, MPI_DOUBLE, 0, 200, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        double t1 = MPI_Wtime();
        if (t1 - t0 < tmin_par) tmin_par = t1 - t0;
    }
    par_time = tmin_par;

    if (rank == 0) {
        double S = (par_time > 0) ? seq_time / par_time : 0.0;
        double E = S / size;
        double err = std::abs(par_avg - seq_avg);
        std::printf("%ld,%d,%.6f,%.6f,%.4f,%.4f,%.3e\n",
                    N, size, seq_time, par_time, S, E, err);
    }

    MPI_Finalize();
    return 0;
}
