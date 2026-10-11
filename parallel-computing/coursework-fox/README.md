# Курсова робота: Паралельний алгоритм Фокса (MPI)

Тема 13. «Дослідження можливості створення паралельного алгоритму для алгоритму
Фокса множення матриць при блочному поділі даних».

Дисципліна: «Технології розподілених систем та паралельні обчислення».

## Структура

```
coursework_fox/
├── src/          — вихідні коди C++ (fox_seq, fox_mpi, fox_tests),
│                   bash-сценарії експериментів, Python-скрипти візуалізації
├── build/        — скомпільовані виконувані файли і логи компіляції
├── results/      — summary.csv, mpi_runs.csv, seq_runs.csv, еталонні ref_*.bin
├── charts/       — двовимірні графіки (PNG)
├── charts3d/     — тривимірні поверхні (PNG)
├── diagrams/     — блок-схема, структура програми, схема решітки
├── screenshots/  — рендер термінальних сесій з реальним виводом
├── report/       — DOCX та PDF курсової роботи
└── README.md
```

## Як зібрати та запустити

```bash
# Збірка
g++   -O2 -std=c++17 -o build/fox_seq  src/fox_seq.cpp
mpic++ -O2 -std=c++17 -o build/fox_mpi  src/fox_mpi.cpp
mpic++ -O2 -std=c++17 -o build/fox_tests src/fox_tests.cpp

# Тести
mpirun --oversubscribe -np 4 ./build/fox_tests

# Експерименти
bash src/run_experiments.sh
bash src/extra_experiments.sh

# Візуалізація
python3 src/make_charts.py
python3 src/make_diagrams.py
python3 src/make_screenshots.py

```

## Результати

Основні результати:
- Усі функціональні тести (I·B, 0·B, детермінований та випадковий) — PASS.
- max|ΔC| ≤ 3·10⁻¹³ у всіх запусках.
- Найкраще прискорення S ≈ 6,58 при P = 9, N = 840.
- Ефективність E > 0,65 для P = 9 на великих N; падіння для P = 16 через
  oversubscribe на системі з 16 ядер.
