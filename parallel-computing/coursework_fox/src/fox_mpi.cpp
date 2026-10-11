// Паралельна реалізація алгоритму Фокса множення матриць C = A*B
// з блочним поділом даних та декартовою решіткою q x q процесів (p = q^2).
//
// Схема алгоритму:
//   1. Побудова декартової двовимірної решітки q x q з періодичним Y.
//   2. Формування row- і column-комунікаторів.
//   3. Розсилання блоків A_{i,j} і B_{i,j} на процеси (i,j) через Scatter.
//   4. Основний цикл з q ітерацій:
//        4.1. На рядку i ведучим є блок A_{i, (i+step) mod q}
//             (broadcast від процесу col == (i+step) mod q в рядку).
//        4.2. Локальне множення C_{i,j} += A_pivot * B_local.
//        4.3. Циклічний зсув блоків B вгору по стовпці на 1 позицію.
//   5. Gather блоків C у процес 0, де формується повна матриця C.

#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

static void fill_matrix(std::vector<double>& m, int n, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int i = 0; i < n * n; ++i) m[i] = dist(rng);
}

static void local_gemm_add(const double* A, const double* B, double* C, int b) {
    // C += A * B,  A, B, C -- квадратні блоки розміром b x b
    for (int i = 0; i < b; ++i) {
        for (int k = 0; k < b; ++k) {
            double a = A[i * b + k];
            for (int j = 0; j < b; ++j) {
                C[i * b + j] += a * B[k * b + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n = 480;
    std::string check_path;
    std::string log_path;
    bool print_csv = false;
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "-n") && i + 1 < argc) n = std::atoi(argv[++i]);
        else if (!std::strcmp(argv[i], "--check") && i + 1 < argc) check_path = argv[++i];
        else if (!std::strcmp(argv[i], "--log") && i + 1 < argc) log_path = argv[++i];
        else if (!std::strcmp(argv[i], "--csv")) print_csv = true;
    }

    // Перевіряємо умову p = q^2
    int q = (int)std::lround(std::sqrt((double)world_size));
    if (q * q != world_size) {
        if (world_rank == 0) {
            std::fprintf(stderr, "[fox] ERROR: число процесів P=%d не є повним квадратом.\n",
                         world_size);
        }
        MPI_Finalize();
        return 2;
    }
    if (n % q != 0) {
        if (world_rank == 0) {
            std::fprintf(stderr, "[fox] ERROR: N=%d не ділиться націло на q=%d.\n", n, q);
        }
        MPI_Finalize();
        return 3;
    }
    int b = n / q;  // розмір блока

    // Декартова топологія q x q, періодична по вертикалі (для зсуву B)
    MPI_Comm grid_comm;
    int dims[2] = {q, q};
    int periods[2] = {0, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid_comm);
    int grid_rank;
    MPI_Comm_rank(grid_comm, &grid_rank);
    int coords[2];
    MPI_Cart_coords(grid_comm, grid_rank, 2, coords);
    int row = coords[0];
    int col = coords[1];

    // Row- і column-комунікатори
    MPI_Comm row_comm, col_comm;
    int row_remain[2] = {0, 1};
    int col_remain[2] = {1, 0};
    MPI_Cart_sub(grid_comm, row_remain, &row_comm);
    MPI_Cart_sub(grid_comm, col_remain, &col_comm);

    // На корені формуємо повні A, B; інші -- лише локальні блоки
    std::vector<double> A_full, B_full, C_full;
    if (grid_rank == 0) {
        A_full.assign((size_t)n * n, 0.0);
        B_full.assign((size_t)n * n, 0.0);
        C_full.assign((size_t)n * n, 0.0);
        fill_matrix(A_full, n, 11u);
        fill_matrix(B_full, n, 29u);
    }

    std::vector<double> A_loc((size_t)b * b, 0.0);
    std::vector<double> B_loc((size_t)b * b, 0.0);
    std::vector<double> C_loc((size_t)b * b, 0.0);
    std::vector<double> A_buf((size_t)b * b, 0.0);  // буфер для broadcast A
    std::vector<double> B_recv((size_t)b * b, 0.0); // буфер зсуву B

    // Розсилання блоків. Використовуємо MPI_Type_vector для "вирізання" блоку b x b
    // з лінійного рядка довжиною n.
    MPI_Datatype block_type, block_resized;
    MPI_Type_vector(b, b, n, MPI_DOUBLE, &block_type);
    MPI_Type_commit(&block_type);
    MPI_Type_create_resized(block_type, 0, (MPI_Aint)(b * sizeof(double)), &block_resized);
    MPI_Type_commit(&block_resized);

    std::vector<int> counts(world_size, 1);
    std::vector<int> displs(world_size, 0);
    // Зсув блоку (i,j) у одиницях блока рядком: i * q * b + j
    for (int p = 0; p < world_size; ++p) {
        int cc[2];
        MPI_Cart_coords(grid_comm, p, 2, cc);
        displs[p] = cc[0] * q * b + cc[1];
    }

    MPI_Barrier(grid_comm);
    double t_scatter_start = MPI_Wtime();

    MPI_Scatterv(grid_rank == 0 ? A_full.data() : nullptr,
                 counts.data(), displs.data(), block_resized,
                 A_loc.data(), b * b, MPI_DOUBLE, 0, grid_comm);
    MPI_Scatterv(grid_rank == 0 ? B_full.data() : nullptr,
                 counts.data(), displs.data(), block_resized,
                 B_loc.data(), b * b, MPI_DOUBLE, 0, grid_comm);

    MPI_Barrier(grid_comm);
    double t_compute_start = MPI_Wtime();

    // Основний цикл Фокса
    int src_up, dst_down;
    MPI_Cart_shift(grid_comm, 0, 1, &src_up, &dst_down); // для col-комунікатора
    // Насправді зсуваємо всередині col_comm (шлях вгору на 1): dest=row-1, src=row+1
    int col_size, col_rank;
    MPI_Comm_size(col_comm, &col_size);
    MPI_Comm_rank(col_comm, &col_rank);
    int shift_src = (col_rank + 1) % col_size;
    int shift_dst = (col_rank - 1 + col_size) % col_size;

    for (int step = 0; step < q; ++step) {
        int pivot = (row + step) % q;
        // Broadcast блоку A з процесу pivot у рядку
        if (col == pivot) {
            std::copy(A_loc.begin(), A_loc.end(), A_buf.begin());
        }
        MPI_Bcast(A_buf.data(), b * b, MPI_DOUBLE, pivot, row_comm);

        // Локальне множення C += A_buf * B_loc
        local_gemm_add(A_buf.data(), B_loc.data(), C_loc.data(), b);

        // Циклічний зсув B вгору
        if (step != q - 1) {
            MPI_Status st;
            MPI_Sendrecv(B_loc.data(), b * b, MPI_DOUBLE, shift_dst, 100,
                         B_recv.data(), b * b, MPI_DOUBLE, shift_src, 100,
                         col_comm, &st);
            B_loc.swap(B_recv);
        }
    }

    MPI_Barrier(grid_comm);
    double t_gather_start = MPI_Wtime();

    // Збір результату в корінь
    MPI_Gatherv(C_loc.data(), b * b, MPI_DOUBLE,
                grid_rank == 0 ? C_full.data() : nullptr,
                counts.data(), displs.data(), block_resized,
                0, grid_comm);

    MPI_Barrier(grid_comm);
    double t_end = MPI_Wtime();

    double t_total = t_end - t_scatter_start;
    double t_compute = t_gather_start - t_compute_start;
    double t_comm_scatter = t_compute_start - t_scatter_start;
    double t_comm_gather = t_end - t_gather_start;

    // Перевірка коректності
    double max_diff = -1.0;
    double checksum = 0.0;
    if (grid_rank == 0) {
        for (double v : C_full) checksum += v;
        if (!check_path.empty()) {
            FILE* f = std::fopen(check_path.c_str(), "rb");
            if (f) {
                int n_ref = 0;
                std::fread(&n_ref, sizeof(int), 1, f);
                if (n_ref == n) {
                    std::vector<double> C_ref((size_t)n * n);
                    std::fread(C_ref.data(), sizeof(double), (size_t)n * n, f);
                    max_diff = 0.0;
                    for (size_t i = 0; i < C_full.size(); ++i) {
                        double d = std::fabs(C_full[i] - C_ref[i]);
                        if (d > max_diff) max_diff = d;
                    }
                }
                std::fclose(f);
            }
        }
    }

    if (grid_rank == 0) {
        if (print_csv) {
            std::printf("%d,%d,%d,%.6f,%.6f,%.6f,%.6f,%.6e,%.6e\n",
                        n, world_size, q, t_total, t_compute,
                        t_comm_scatter, t_comm_gather, checksum, max_diff);
        } else {
            std::printf("[fox] N=%d P=%d q=%d b=%d  total=%.6f  compute=%.6f  "
                        "scatter=%.6f  gather=%.6f  checksum=%.6e  max_diff=%.3e\n",
                        n, world_size, q, b,
                        t_total, t_compute, t_comm_scatter, t_comm_gather,
                        checksum, max_diff);
        }
        if (!log_path.empty()) {
            FILE* f = std::fopen(log_path.c_str(), "a");
            if (f) {
                std::fprintf(f, "%d,%d,%d,%.6f,%.6f,%.6f,%.6f,%.6e,%.6e\n",
                             n, world_size, q, t_total, t_compute,
                             t_comm_scatter, t_comm_gather, checksum, max_diff);
                std::fclose(f);
            }
        }
    }

    MPI_Type_free(&block_resized);
    MPI_Type_free(&block_type);
    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&grid_comm);
    MPI_Finalize();
    return 0;
}
