// Лабораторна 3, варіант 15. Паралельне сортування:
//   алгоритм 2 — швидке сортування (парадигма гіперкуба) — N за варіантом = 10000;
//   алгоритм 3 — парно-непарне сортування по блоках           — N за варіантом = 15000.
// Комунікація — блокуючі MPI_Send / MPI_Recv / MPI_Sendrecv,
// плюс колективи Scatterv / Gatherv / Bcast / Comm_split.
//
// Програма приймає:
//   argv[1] = N (розмір масиву);
//   argv[2] = alg  (2 = quick, 3 = odd-even).

#include <mpi.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

static std::vector<double> merge_sorted(const std::vector<double>& a,
                                        const std::vector<double>& b) {
    std::vector<double> c(a.size() + b.size());
    std::merge(a.begin(), a.end(), b.begin(), b.end(), c.begin());
    return c;
}

// compare_split за правилом keep_low:
//   обмін блоками із сусідом peer, потім злиття, далі залишаємо
//   нижню (keep_low = true) або верхню частину злитого масиву.
static void compare_split(int peer, std::vector<double>& my, bool keep_low) {
    long my_n = (long)my.size();
    long peer_n = 0;
    MPI_Status st;
    MPI_Sendrecv(&my_n, 1, MPI_LONG, peer, 1,
                 &peer_n, 1, MPI_LONG, peer, 1,
                 MPI_COMM_WORLD, &st);
    std::vector<double> theirs(peer_n);
    MPI_Sendrecv(my.data(), (int)my_n, MPI_DOUBLE, peer, 2,
                 theirs.data(), (int)peer_n, MPI_DOUBLE, peer, 2,
                 MPI_COMM_WORLD, &st);
    auto merged = merge_sorted(my, theirs);
    if (keep_low) my.assign(merged.begin(), merged.begin() + my_n);
    else          my.assign(merged.end() - my_n, merged.end());
}

// Парно-непарне сортування між P блоками.
static void odd_even(std::vector<double>& my, int rank, int np) {
    std::sort(my.begin(), my.end()); // локальна передсортировка
    for (int i = 0; i < np; ++i) {
        int peer = -1;
        bool keep_low = false;
        if (i % 2 == 0) { // парна ітерація: пари (0,1)(2,3)...
            peer = (rank % 2 == 0) ? rank + 1 : rank - 1;
            keep_low = (rank % 2 == 0);
        } else {          // непарна: пари (1,2)(3,4)...
            peer = (rank % 2 == 1) ? rank + 1 : rank - 1;
            keep_low = (rank % 2 == 1);
        }
        if (peer < 0 || peer >= np) continue;
        compare_split(peer, my, keep_low);
    }
}

// Паралельне швидке сортування на гіперкубі (p = 2^k).
// На кожному рівні рекурсії всі процеси у поточному sub-комунікаторі
// отримують ведучий елемент (медіана блоку з rank=0 цього sub-comm),
// розбивають свій локальний блок на «менші» і «більші», обмінюються
// блоками з парним процесом за протилежним бітом, потім sub-комунікатор
// ділиться на дві половини і процес повторюється.
static void parallel_quick(std::vector<double>& my, MPI_Comm comm) {
    int rank, np;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &np);
    if (np == 1) { std::sort(my.begin(), my.end()); return; }

    double pivot = 0.0;
    if (rank == 0 && !my.empty()) {
        std::vector<double> t = my;
        std::nth_element(t.begin(), t.begin() + t.size() / 2, t.end());
        pivot = t[t.size() / 2];
    }
    MPI_Bcast(&pivot, 1, MPI_DOUBLE, 0, comm);

    std::vector<double> low, high;
    for (double v : my) (v < pivot ? low : high).push_back(v);

    int half = np / 2;
    int peer = (rank < half) ? rank + half : rank - half;
    bool send_high = (rank < half);
    auto& sendBuf = send_high ? high : low;
    auto& keepBuf = send_high ? low : high;

    long sn = (long)sendBuf.size(), rn = 0;
    MPI_Status st;
    MPI_Sendrecv(&sn, 1, MPI_LONG, peer, 10,
                 &rn, 1, MPI_LONG, peer, 10, comm, &st);
    std::vector<double> recvBuf(rn);
    MPI_Sendrecv(sendBuf.data(), (int)sn, MPI_DOUBLE, peer, 11,
                 recvBuf.data(), (int)rn, MPI_DOUBLE, peer, 11, comm, &st);
    keepBuf.insert(keepBuf.end(), recvBuf.begin(), recvBuf.end());
    my = std::move(keepBuf);

    int color = (rank < half) ? 0 : 1;
    MPI_Comm sub;
    MPI_Comm_split(comm, color, rank, &sub);
    parallel_quick(my, sub);
    MPI_Comm_free(&sub);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    long N   = (argc > 1) ? std::atol(argv[1]) : 15000;
    int  alg = (argc > 2) ? std::atoi(argv[2]) : 2; // 2 = quick, 3 = odd-even

    // Майстер готує початковий масив
    std::vector<double> data;
    if (rank == 0) {
        data.resize(N);
        std::mt19937_64 rng(15 + (unsigned)alg);
        std::uniform_real_distribution<double> d(-1e6, 1e6);
        for (long i = 0; i < N; ++i) data[i] = d(rng);
    }

    // Послідовний еталон на rank 0 (за std::sort — найкращий послідовний).
    double tseq = 0.0;
    std::vector<double> seq_sorted;
    if (rank == 0) {
        seq_sorted = data;
        double t0 = MPI_Wtime();
        std::sort(seq_sorted.begin(), seq_sorted.end());
        tseq = MPI_Wtime() - t0;
    }

    // Розподіл блоків по процесах: Scatterv (враховує залишок).
    std::vector<int> counts(np), displs(np);
    long base = N / np, rem = N % np;
    long cum = 0;
    for (int p = 0; p < np; ++p) {
        counts[p] = (int)(base + (p < rem ? 1 : 0));
        displs[p] = (int)cum;
        cum += counts[p];
    }
    std::vector<double> my(counts[rank]);
    MPI_Scatterv(rank == 0 ? data.data() : nullptr,
                 counts.data(), displs.data(), MPI_DOUBLE,
                 my.data(), counts[rank], MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // Паралельне виконання.
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    if (alg == 2) {
        parallel_quick(my, MPI_COMM_WORLD);
    } else {
        odd_even(my, rank, np);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double tpar = MPI_Wtime() - t0;

    // Для quick sort обсяг «my» на кожному процесі може змінитися —
    // збираємо реальні розміри та зливаємо через Gatherv.
    int my_final = (int)my.size();
    std::vector<int> final_counts(np), final_displs(np);
    MPI_Gather(&my_final, 1, MPI_INT, final_counts.data(), 1, MPI_INT,
               0, MPI_COMM_WORLD);
    std::vector<double> gathered;
    if (rank == 0) {
        long tot = 0;
        for (int p = 0; p < np; ++p) {
            final_displs[p] = (int)tot;
            tot += final_counts[p];
        }
        gathered.resize(tot);
    }
    MPI_Gatherv(my.data(), my_final, MPI_DOUBLE,
                gathered.data(), final_counts.data(), final_displs.data(),
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Верифікація: gathered порівнюється поелементно з seq_sorted,
        // додатково перевіряється загальна кількість елементів і
        // відсортованість. Лише при збігу всіх трьох умов verify = OK.
        bool size_ok = (gathered.size() == (size_t)N);
        bool sorted_ok = std::is_sorted(gathered.begin(), gathered.end());
        bool match_ok = size_ok;
        if (size_ok) {
            for (long i = 0; i < N; ++i) {
                if (gathered[i] != seq_sorted[i]) { match_ok = false; break; }
            }
        }
        bool ok = size_ok && sorted_ok && match_ok;
        double S = tseq / tpar;
        double E = S / np;
        // формат: alg,N,P,seq_t,par_t,S,E,verify
        std::printf("%d,%ld,%d,%.6f,%.6f,%.4f,%.4f,%s\n",
                    alg, N, np, tseq, tpar, S, E, ok ? "OK" : "FAIL");
    }
    MPI_Finalize();
    return 0;
}
