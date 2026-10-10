// Лабораторна 2, варіант 15, задача 3.
// Пошук максимального і мінімального значень у заданому наборі числових даних.
// Реалізація: блокуючий двоточковий обмін MPI_Send / MPI_Recv
// (майстер-процес розподіляє смуги та збирає локальні min/max).

#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <limits>
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
        std::mt19937_64 rng(15ULL * 17);
        std::uniform_real_distribution<double> d(-1e6, 1e6);
        for (long i = 0; i < N; ++i) data[i] = d(rng);
    }

    // послідовний еталон
    double seq_time = 0.0, seq_min = 0.0, seq_max = 0.0;
    if (rank == 0) {
        double tmin = 1e30;
        for (int r = 0; r < repeats; ++r) {
            double t0 = MPI_Wtime();
            double mn =  std::numeric_limits<double>::infinity();
            double mx = -std::numeric_limits<double>::infinity();
            for (long i = 0; i < N; ++i) {
                if (data[i] < mn) mn = data[i];
                if (data[i] > mx) mx = data[i];
            }
            double t1 = MPI_Wtime();
            seq_min = mn; seq_max = mx;
            if (t1 - t0 < tmin) tmin = t1 - t0;
        }
        seq_time = tmin;
    }

    long base = N / size;
    long rem  = N % size;

    double par_time = 0.0, par_min = 0.0, par_max = 0.0;
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
                MPI_Send(&cnt, 1, MPI_LONG, p, 10, MPI_COMM_WORLD);
                MPI_Send(data.data() + offset, (int)cnt, MPI_DOUBLE, p, 11, MPI_COMM_WORLD);
                offset += cnt;
            }
        } else {
            MPI_Status st;
            MPI_Recv(&my_n, 1, MPI_LONG, 0, 10, MPI_COMM_WORLD, &st);
            myD.resize(my_n);
            MPI_Recv(myD.data(), (int)my_n, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD, &st);
        }

        double lmn =  std::numeric_limits<double>::infinity();
        double lmx = -std::numeric_limits<double>::infinity();
        for (long i = 0; i < my_n; ++i) {
            if (myD[i] < lmn) lmn = myD[i];
            if (myD[i] > lmx) lmx = myD[i];
        }

        if (rank == 0) {
            double mn = lmn, mx = lmx;
            for (int p = 1; p < size; ++p) {
                double pmn, pmx;
                MPI_Status st;
                MPI_Recv(&pmn, 1, MPI_DOUBLE, p, 20, MPI_COMM_WORLD, &st);
                MPI_Recv(&pmx, 1, MPI_DOUBLE, p, 21, MPI_COMM_WORLD, &st);
                if (pmn < mn) mn = pmn;
                if (pmx > mx) mx = pmx;
            }
            par_min = mn; par_max = mx;
        } else {
            MPI_Send(&lmn, 1, MPI_DOUBLE, 0, 20, MPI_COMM_WORLD);
            MPI_Send(&lmx, 1, MPI_DOUBLE, 0, 21, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        double t1 = MPI_Wtime();
        if (t1 - t0 < tmin_par) tmin_par = t1 - t0;
    }
    par_time = tmin_par;

    if (rank == 0) {
        double S = (par_time > 0) ? seq_time / par_time : 0.0;
        double E = S / size;
        bool ok = (par_min == seq_min) && (par_max == seq_max);
        std::printf("%ld,%d,%.6f,%.6f,%.4f,%.4f,%s\n",
                    N, size, seq_time, par_time, S, E, ok ? "OK" : "MISMATCH");
    }

    MPI_Finalize();
    return 0;
}
