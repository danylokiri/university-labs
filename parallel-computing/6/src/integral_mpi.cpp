// Лабораторна 6, варіант 15.
// Інтеграл: I = ∫₀^π (x² - 3x + 2)·cos(x) dx.
// Аналітичне значення:
//     F(x) = (x² - 3x)·sin(x) + (2x - 3)·cos(x),
//     I = F(π) − F(0) = (3 − 2π) − (−3) = 6 − 2π ≈ −0.28318530717958623.
//
// Дві реалізації в одному виконуваному файлі: метод трапецій та метод
// Сімпсона. Розпаралелення: кожен процес обчислює свій підінтервал;
// параметри (a, b, n) майстер розсилає через MPI_Bcast; часткові суми
// зводяться до глобальної через MPI_Reduce з оператором MPI_SUM.

#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

static double f(double x) { return (x*x - 3.0*x + 2.0) * std::cos(x); }

// Трапеції на підінтервалі [a, b] з n підкроками.
static double trapezoid(double a, double b, long n) {
    double h = (b - a) / (double)n;
    double s = 0.5 * (f(a) + f(b));
    for (long i = 1; i < n; ++i) s += f(a + (double)i * h);
    return s * h;
}

// Сімпсон на [a, b] з парним n.
static double simpson(double a, double b, long n) {
    if (n % 2) ++n;
    double h = (b - a) / (double)n;
    double s_end  = f(a) + f(b);
    double s_odd  = 0.0, s_even = 0.0;
    for (long i = 1; i < n; ++i) {
        double v = f(a + (double)i * h);
        if (i & 1) s_odd += v; else s_even += v;
    }
    return (s_end + 4.0*s_odd + 2.0*s_even) * h / 3.0;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    double a = 0.0, b = M_PI;
    long   n = (argc > 1) ? std::atol(argv[1]) : 10000000;
    int method = (argc > 2) ? std::atoi(argv[2]) : 0; // 0 = trap, 1 = simpson

    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_LONG,   0, MPI_COMM_WORLD);

    // ---- послідовний еталон на rank 0 ----
    double tseq = 0.0;
    volatile double Iseq = 0.0;   // volatile — щоб компілятор не викинув виклик при -O2
    if (rank == 0) {
        double t0 = MPI_Wtime();
        Iseq = (method == 0) ? trapezoid(a, b, n) : simpson(a, b, n);
        tseq = MPI_Wtime() - t0;
    }

    // ---- паралельне обчислення ----
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    double len = (b - a) / (double)np;
    long   ln  = n / np;
    double la  = a + (double)rank * len;
    double lb  = la + len;
    double local = (method == 0) ? trapezoid(la, lb, ln) : simpson(la, lb, ln);
    double total = 0.0;
    MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    double tpar = MPI_Wtime() - t0;

    if (rank == 0) {
        double exact = 6.0 - 2.0 * M_PI;
        double err_par = std::abs(total - exact);
        double S = tseq / tpar, E = S / np;
        double iseq_val = Iseq;  // розпаковуємо volatile у звичайну
        (void)iseq_val;
        // формат: method,n,P,seq_t,par_t,S,E,I_par,err
        std::printf("%d,%ld,%d,%.6f,%.6f,%.4f,%.4f,%.12f,%.3e\n",
                    method, n, np, tseq, tpar, S, E, total, err_par);
    }
    MPI_Finalize();
    return 0;
}
