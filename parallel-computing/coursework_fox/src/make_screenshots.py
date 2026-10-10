#!/usr/bin/env python3
# Генерація скріншотів термінальних сесій на базі реальних запусків.
# Кожен скріншот містить реальний stdout виконаних команд.

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT.parent / "_common"))
from term_screenshot import render_screenshot, run  # type: ignore

OUT = ROOT / "screenshots"
OUT.mkdir(exist_ok=True)

CWD = "."
SHORT = "…/coursework_fox"


def shot(name, commands, title="bash — Ubuntu (WSL)"):
    sessions = []
    for path_prompt, cmd in commands:
        out = run(cmd, CWD)
        # Обрізаємо надто довгі блоки, щоб картинка не була нескінченною
        lines = out.splitlines()
        if len(lines) > 28:
            lines = lines[:26] + ["... (обрізано для скріншота)"]
        out = "\n".join(lines)
        sessions.append((path_prompt, cmd, out))
    render_screenshot(str(OUT / name), sessions, title_text=title)


# 1. Компіляція
shot("01_compile.png", [
    (SHORT, "mpic++ -O2 -std=c++17 -o build/fox_mpi src/fox_mpi.cpp && echo OK"),
    (SHORT, "g++ -O2 -std=c++17 -o build/fox_seq src/fox_seq.cpp && echo OK"),
    (SHORT, "ls -l build/ | awk 'NR>1 {print $1, $5, $9}'"),
])

# 2. Тести
shot("02_tests.png", [
    (SHORT, "mpirun --oversubscribe -np 4 ./build/fox_tests"),
    (SHORT, "mpirun --oversubscribe -np 9 ./build/fox_tests"),
])

# 3. Запуск послідовної версії
shot("03_seq_run.png", [
    (SHORT, "./build/fox_seq -n 480"),
    (SHORT, "./build/fox_seq -n 720"),
])

# 4. Запуск MPI (P=4)
shot("04_mpi_p4.png", [
    (SHORT, "mpirun --oversubscribe -np 4 ./build/fox_mpi -n 480"),
    (SHORT, "mpirun --oversubscribe -np 4 ./build/fox_mpi -n 720"),
])

# 5. Запуск MPI (P=9)
shot("05_mpi_p9.png", [
    (SHORT, "mpirun --oversubscribe -np 9 ./build/fox_mpi -n 720"),
    (SHORT, "mpirun --oversubscribe -np 9 ./build/fox_mpi -n 1200"),
])

# 6. Перевірка коректності (з еталонним файлом)
shot("06_correctness.png", [
    (SHORT, "./build/fox_seq -n 480 --save results/ref_480.bin"),
    (SHORT, "mpirun --oversubscribe -np 4 ./build/fox_mpi -n 480 --check results/ref_480.bin"),
    (SHORT, "mpirun --oversubscribe -np 9 ./build/fox_mpi -n 480 --check results/ref_480.bin"),
])

# 7. Підсумкова таблиця summary.csv
shot("07_summary.png", [
    (SHORT, "column -t -s, results/summary.csv | head -28"),
])

# 8. Структура проекту
shot("08_tree.png", [
    (SHORT, "ls"),
    (SHORT, "ls src/"),
    (SHORT, "ls results/ | head -15"),
    (SHORT, "ls charts/"),
    (SHORT, "ls charts3d/"),
    (SHORT, "ls diagrams/"),
])

# 9. Докладний лог одного запуску з розбивкою
shot("09_breakdown.png", [
    (SHORT, "mpirun --oversubscribe -np 9 ./build/fox_mpi -n 960 --check results/ref_960.bin"),
])

print("Screenshots saved to", OUT)
