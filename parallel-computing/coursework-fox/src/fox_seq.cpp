// Послідовна еталонна реалізація множення матриць C = A*B
// Використовується для перевірки коректності паралельного алгоритму Фокса
// та як базис T1 для обчислення прискорення S = T1/Tp.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <random>
#include <vector>
#include <string>

static void fill_matrix(std::vector<double>& m, int n, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int i = 0; i < n * n; ++i) m[i] = dist(rng);
}

static void multiply(const std::vector<double>& A,
                     const std::vector<double>& B,
                     std::vector<double>& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a = A[i * n + k];
            for (int j = 0; j < n; ++j) {
                C[i * n + j] += a * B[k * n + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int n = 480;
    std::string save_path;
    bool print_csv = false;
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "-n") && i + 1 < argc) n = std::atoi(argv[++i]);
        else if (!std::strcmp(argv[i], "--save") && i + 1 < argc) save_path = argv[++i];
        else if (!std::strcmp(argv[i], "--csv")) print_csv = true;
    }
    if (n <= 0) { std::fprintf(stderr, "Invalid N=%d\n", n); return 1; }

    std::vector<double> A(n * n), B(n * n), C(n * n, 0.0);
    fill_matrix(A, n, 11u);
    fill_matrix(B, n, 29u);

    auto t0 = std::chrono::steady_clock::now();
    multiply(A, B, C, n);
    auto t1 = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(t1 - t0).count();

    // Контрольна сума для перевірки коректності в інших запусках
    double checksum = 0.0;
    for (int i = 0; i < n * n; ++i) checksum += C[i];

    if (print_csv) {
        std::printf("%d,%.6f,%.6e\n", n, dt, checksum);
    } else {
        std::printf("[seq] N=%d  time=%.6f s  checksum=%.6e\n", n, dt, checksum);
    }

    if (!save_path.empty()) {
        FILE* f = std::fopen(save_path.c_str(), "wb");
        if (f) {
            std::fwrite(&n, sizeof(int), 1, f);
            std::fwrite(C.data(), sizeof(double), (size_t)n * n, f);
            std::fclose(f);
        }
    }
    return 0;
}
