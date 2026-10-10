// Набір функціональних тестів реалізації алгоритму Фокса.
// Виконується за допомогою mpirun з P = q^2 процесів.
//
// Крайні випадки, які перевіряються:
//   1. Одинична матриця: I * A = A
//   2. Нульова матриця: 0 * A = 0
//   3. Матриці з детермінованим заповненням малого розміру,
//      де результат обчислюється тривіально на корені.
//   4. Сумісність q x q розкладу (p повинно дорівнювати q^2).
//
// Програма друкує короткий звіт "PASS"/"FAIL" для кожного тесту на корені.

#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

static void local_gemm_add(const double* A, const double* B, double* C, int b) {
    for (int i = 0; i < b; ++i)
        for (int k = 0; k < b; ++k) {
            double a = A[i * b + k];
            for (int j = 0; j < b; ++j)
                C[i * b + j] += a * B[k * b + j];
        }
}

static void seq_multiply(const std::vector<double>& A,
                         const std::vector<double>& B,
                         std::vector<double>& C, int n) {
    std::fill(C.begin(), C.end(), 0.0);
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            double a = A[i * n + k];
            for (int j = 0; j < n; ++j)
                C[i * n + j] += a * B[k * n + j];
        }
}

static double run_fox(int n, int q, MPI_Comm grid_comm, MPI_Comm row_comm, MPI_Comm col_comm,
                     const std::vector<double>& A_full,
                     const std::vector<double>& B_full,
                     std::vector<double>& C_full) {
    int grid_rank;
    MPI_Comm_rank(grid_comm, &grid_rank);
    int world_size;
    MPI_Comm_size(grid_comm, &world_size);
    int coords[2];
    MPI_Cart_coords(grid_comm, grid_rank, 2, coords);
    int row = coords[0];
    int col = coords[1];
    int b = n / q;

    std::vector<double> A_loc((size_t)b * b, 0.0);
    std::vector<double> B_loc((size_t)b * b, 0.0);
    std::vector<double> C_loc((size_t)b * b, 0.0);
    std::vector<double> A_buf((size_t)b * b, 0.0);
    std::vector<double> B_recv((size_t)b * b, 0.0);

    MPI_Datatype block_type, block_resized;
    MPI_Type_vector(b, b, n, MPI_DOUBLE, &block_type);
    MPI_Type_commit(&block_type);
    MPI_Type_create_resized(block_type, 0, (MPI_Aint)(b * sizeof(double)), &block_resized);
    MPI_Type_commit(&block_resized);

    std::vector<int> counts(world_size, 1);
    std::vector<int> displs(world_size, 0);
    for (int p = 0; p < world_size; ++p) {
        int cc[2];
        MPI_Cart_coords(grid_comm, p, 2, cc);
        displs[p] = cc[0] * q * b + cc[1];
    }

    MPI_Scatterv(grid_rank == 0 ? A_full.data() : nullptr,
                 counts.data(), displs.data(), block_resized,
                 A_loc.data(), b * b, MPI_DOUBLE, 0, grid_comm);
    MPI_Scatterv(grid_rank == 0 ? B_full.data() : nullptr,
                 counts.data(), displs.data(), block_resized,
                 B_loc.data(), b * b, MPI_DOUBLE, 0, grid_comm);

    int col_size, col_rank;
    MPI_Comm_size(col_comm, &col_size);
    MPI_Comm_rank(col_comm, &col_rank);
    int shift_src = (col_rank + 1) % col_size;
    int shift_dst = (col_rank - 1 + col_size) % col_size;

    for (int step = 0; step < q; ++step) {
        int pivot = (row + step) % q;
        if (col == pivot) std::copy(A_loc.begin(), A_loc.end(), A_buf.begin());
        MPI_Bcast(A_buf.data(), b * b, MPI_DOUBLE, pivot, row_comm);
        local_gemm_add(A_buf.data(), B_loc.data(), C_loc.data(), b);
        if (step != q - 1) {
            MPI_Status st;
            MPI_Sendrecv(B_loc.data(), b * b, MPI_DOUBLE, shift_dst, 100,
                         B_recv.data(), b * b, MPI_DOUBLE, shift_src, 100,
                         col_comm, &st);
            B_loc.swap(B_recv);
        }
    }

    MPI_Gatherv(C_loc.data(), b * b, MPI_DOUBLE,
                grid_rank == 0 ? C_full.data() : nullptr,
                counts.data(), displs.data(), block_resized,
                0, grid_comm);

    MPI_Type_free(&block_resized);
    MPI_Type_free(&block_type);

    double max_diff = 0.0;
    return max_diff;
}

static bool compare(const std::vector<double>& X, const std::vector<double>& Y, double tol) {
    if (X.size() != Y.size()) return false;
    for (size_t i = 0; i < X.size(); ++i)
        if (std::fabs(X[i] - Y[i]) > tol) return false;
    return true;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int q = (int)std::lround(std::sqrt((double)world_size));
    if (q * q != world_size) {
        if (world_rank == 0) std::fprintf(stderr, "need P=q^2\n");
        MPI_Finalize();
        return 2;
    }
    MPI_Comm grid_comm, row_comm, col_comm;
    int dims[2] = {q, q};
    int periods[2] = {0, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid_comm);
    int row_remain[2] = {0, 1};
    int col_remain[2] = {1, 0};
    MPI_Cart_sub(grid_comm, row_remain, &row_comm);
    MPI_Cart_sub(grid_comm, col_remain, &col_comm);

    auto report = [&](const char* name, bool ok) {
        if (world_rank == 0) std::printf("[test] %-40s : %s\n", name, ok ? "PASS" : "FAIL");
    };

    // Тест 1. Одинична матриця: I * A = A
    {
        int n = 4 * q;
        std::vector<double> A, B, C(n * n, 0.0), C_ref(n * n, 0.0);
        if (world_rank == 0) {
            A.assign(n * n, 0.0);
            for (int i = 0; i < n; ++i) A[i * n + i] = 1.0;
            B.assign(n * n, 0.0);
            for (int i = 0; i < n * n; ++i) B[i] = (double)((i * 7 + 3) % 17 - 8);
            C_ref = B;
        }
        run_fox(n, q, grid_comm, row_comm, col_comm, A, B, C);
        bool ok = (world_rank != 0) || compare(C, C_ref, 1e-9);
        int ok_i = ok ? 1 : 0;
        MPI_Bcast(&ok_i, 1, MPI_INT, 0, MPI_COMM_WORLD);
        report("Identity  I * B == B", ok_i != 0);
    }

    // Тест 2. Нульова матриця: 0 * B = 0
    {
        int n = 4 * q;
        std::vector<double> A, B, C(n * n, 0.0), C_ref(n * n, 0.0);
        if (world_rank == 0) {
            A.assign(n * n, 0.0);
            B.assign(n * n, 0.0);
            for (int i = 0; i < n * n; ++i) B[i] = (double)((i * 13 + 5) % 29);
            C_ref.assign(n * n, 0.0);
        }
        run_fox(n, q, grid_comm, row_comm, col_comm, A, B, C);
        bool ok = (world_rank != 0) || compare(C, C_ref, 1e-12);
        int ok_i = ok ? 1 : 0;
        MPI_Bcast(&ok_i, 1, MPI_INT, 0, MPI_COMM_WORLD);
        report("Zero matrix  0 * B == 0", ok_i != 0);
    }

    // Тест 3. Малий детермінований приклад, порівняння з послідовним
    {
        int n = 3 * q; // малий розмір, кратний q
        std::vector<double> A(n * n, 0.0), B(n * n, 0.0), C(n * n, 0.0), C_ref(n * n, 0.0);
        if (world_rank == 0) {
            for (int i = 0; i < n * n; ++i) A[i] = (double)((i * 3 + 1) % 11);
            for (int i = 0; i < n * n; ++i) B[i] = (double)((i * 5 + 2) % 13);
            seq_multiply(A, B, C_ref, n);
        }
        run_fox(n, q, grid_comm, row_comm, col_comm, A, B, C);
        bool ok = (world_rank != 0) || compare(C, C_ref, 1e-9);
        int ok_i = ok ? 1 : 0;
        MPI_Bcast(&ok_i, 1, MPI_INT, 0, MPI_COMM_WORLD);
        report("Small deterministic vs sequential", ok_i != 0);
    }

    // Тест 4. Випадкова матриця більшого розміру, порівняння з послідовним
    {
        int n = 6 * q;
        std::vector<double> A(n * n, 0.0), B(n * n, 0.0), C(n * n, 0.0), C_ref(n * n, 0.0);
        if (world_rank == 0) {
            for (int i = 0; i < n * n; ++i) A[i] = std::sin(0.01 * i);
            for (int i = 0; i < n * n; ++i) B[i] = std::cos(0.02 * i);
            seq_multiply(A, B, C_ref, n);
        }
        run_fox(n, q, grid_comm, row_comm, col_comm, A, B, C);
        bool ok = true;
        if (world_rank == 0) {
            double maxd = 0.0;
            for (size_t i = 0; i < C.size(); ++i) {
                double d = std::fabs(C[i] - C_ref[i]);
                if (d > maxd) maxd = d;
            }
            ok = maxd < 1e-7;
            std::printf("[test] Random N=%d max|diff|=%.3e\n", n, maxd);
        }
        int ok_i = ok ? 1 : 0;
        MPI_Bcast(&ok_i, 1, MPI_INT, 0, MPI_COMM_WORLD);
        report("Random continuous vs sequential", ok_i != 0);
    }

    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&grid_comm);
    MPI_Finalize();
    return 0;
}
