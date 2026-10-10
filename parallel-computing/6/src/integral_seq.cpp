// Лабораторна 6, варіант 15 — ПОСЛІДОВНА (чисто однопроцесна) програма.
// Використовується для окремих замірів послідовного часу та для
// доказів виконання у формі скріншотів терміналу, незалежно від MPI.
//
// Інтеграл: I = ∫₀^π (x² − 3x + 2) · cos(x) dx.
// Аналітичне значення: I = 6 − 2π ≈ −0.28318530717958623.

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using Clock = std::chrono::high_resolution_clock;

static inline double f(double x) { return (x*x - 3.0*x + 2.0) * std::cos(x); }

static double trapezoid(double a, double b, long n) {
    double h = (b - a) / (double)n;
    double s = 0.5 * (f(a) + f(b));
    for (long i = 1; i < n; ++i) s += f(a + (double)i * h);
    return s * h;
}

static double simpson(double a, double b, long n) {
    if (n % 2) ++n;
    double h = (b - a) / (double)n;
    double s_end = f(a) + f(b);
    double s_odd = 0.0, s_even = 0.0;
    for (long i = 1; i < n; ++i) {
        double v = f(a + (double)i * h);
        if (i & 1) s_odd += v; else s_even += v;
    }
    return (s_end + 4.0*s_odd + 2.0*s_even) * h / 3.0;
}

int main(int argc, char** argv) {
    long n = (argc > 1) ? std::atol(argv[1]) : 10000000;
    int method = (argc > 2) ? std::atoi(argv[2]) : 0; // 0 = трапеції, 1 = Сімпсон

    const double a = 0.0, b = M_PI;
    const double exact = 6.0 - 2.0 * M_PI;

    auto t0 = Clock::now();
    volatile double I = (method == 0) ? trapezoid(a, b, n) : simpson(a, b, n);
    double t = std::chrono::duration<double>(Clock::now() - t0).count();
    double Iv = I;

    const char* name = (method == 0) ? "trapezoid" : "simpson";
    double err = std::abs(Iv - exact);
    std::printf("method=%s  n=%ld  I=%.12f  exact=%.12f  |err|=%.3e  time=%.6f s\n",
                name, n, Iv, exact, err, t);
    return 0;
}
