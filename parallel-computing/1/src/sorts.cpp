// Лабораторна робота 1
// Послідовні алгоритми сортування: бульбашкове, Шелла, швидке.
// Вимір часу виконання та запис у CSV.

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>

using Clock = std::chrono::high_resolution_clock;

static void bubble_sort(std::vector<double>& a) {
    size_t n = a.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < n - i; ++j) {
            if (a[j] > a[j+1]) { std::swap(a[j], a[j+1]); swapped = true; }
        }
        if (!swapped) break;
    }
}

static void shell_sort(std::vector<double>& a) {
    size_t n = a.size();
    for (size_t gap = n/2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < n; ++i) {
            double tmp = a[i];
            size_t j = i;
            while (j >= gap && a[j-gap] > tmp) {
                a[j] = a[j-gap];
                j -= gap;
            }
            a[j] = tmp;
        }
    }
}

static void quick_sort_rec(std::vector<double>& a, long lo, long hi) {
    while (lo < hi) {
        double pivot = a[lo + (hi-lo)/2];
        long i = lo, j = hi;
        while (i <= j) {
            while (a[i] < pivot) ++i;
            while (a[j] > pivot) --j;
            if (i <= j) { std::swap(a[i], a[j]); ++i; --j; }
        }
        if (j - lo < hi - i) {
            if (lo < j) quick_sort_rec(a, lo, j);
            lo = i;
        } else {
            if (i < hi) quick_sort_rec(a, i, hi);
            hi = j;
        }
    }
}
static void quick_sort(std::vector<double>& a) {
    if (!a.empty()) quick_sort_rec(a, 0, (long)a.size()-1);
}

int main(int argc, char** argv) {
    std::vector<size_t> sizes = {1000, 2000, 5000, 10000, 20000, 50000};
    if (argc > 1) {
        sizes.clear();
        for (int i = 1; i < argc; ++i) sizes.push_back(std::stoul(argv[i]));
    }

    std::mt19937 rng(15);
    std::uniform_real_distribution<double> dist(-1e6, 1e6);

    std::ofstream csv("results/sequential_times.csv");
    csv << "n,bubble_s,shell_s,quick_s\n";

    std::cout << std::fixed;
    std::cout.precision(6);
    std::cout << "      n      bubble(s)    shell(s)    quick(s)\n";

    for (size_t n : sizes) {
        std::vector<double> base(n);
        for (auto& x : base) x = dist(rng);

        auto v1 = base;
        auto t0 = Clock::now();
        bubble_sort(v1);
        double tb = std::chrono::duration<double>(Clock::now()-t0).count();

        auto v2 = base;
        t0 = Clock::now();
        shell_sort(v2);
        double ts = std::chrono::duration<double>(Clock::now()-t0).count();

        auto v3 = base;
        t0 = Clock::now();
        quick_sort(v3);
        double tq = std::chrono::duration<double>(Clock::now()-t0).count();

        bool ok = std::is_sorted(v1.begin(), v1.end())
               && std::is_sorted(v2.begin(), v2.end())
               && std::is_sorted(v3.begin(), v3.end());
        if (!ok) { std::cerr << "SORT VERIFICATION FAILED at n=" << n << "\n"; return 1; }

        std::cout << n << "  " << tb << "  " << ts << "  " << tq << "\n";
        csv << n << "," << tb << "," << ts << "," << tq << "\n";
    }
    return 0;
}
