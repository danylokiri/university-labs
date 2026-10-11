#!/usr/bin/env python3
# Побудова блок-схем алгоритму Фокса та структури програми
# засобами matplotlib (без зовнішніх залежностей типу graphviz).

from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

ROOT = Path(__file__).resolve().parent.parent
OUT  = ROOT / "diagrams"
OUT.mkdir(exist_ok=True)

# ------------------------ 1. Блок-схема алгоритму Фокса ------------------------

def flowchart_fox():
    # Єдина вертикальна колонка. Зворотна стрілка циклу — ліворуч від неї.
    # Вихід з циклу — вниз. Завершення (Gather, max|ΔC|, вивід, Finalize) теж
    # у тій самій колонці, без паралельних гілок.
    fig, ax = plt.subplots(figsize=(9.5, 14.5))
    Y_TOP = 40
    ax.set_xlim(0, 12)
    ax.set_ylim(0, Y_TOP)
    ax.axis("off")

    CENTER = 6.5
    LEFT_LANE = 2.0      # зворотна стрілка циклу
    ERROR_X = 10.8       # блок помилкового виходу

    def box(x, y, w, h, text, shape="rect", color="#e6f2ff"):
        if shape == "rect":
            p = patches.FancyBboxPatch((x - w/2, y - h/2), w, h,
                boxstyle="round,pad=0.15", linewidth=1.4,
                edgecolor="#2244aa", facecolor=color)
        elif shape == "oval":
            p = patches.Ellipse((x, y), w, h, linewidth=1.4,
                edgecolor="#a33", facecolor="#ffe8e8")
        elif shape == "diamond":
            p = patches.Polygon([(x, y+h/2), (x+w/2, y), (x, y-h/2), (x-w/2, y)],
                linewidth=1.4, edgecolor="#a37", facecolor="#fff2e6")
        elif shape == "par":
            p = patches.Polygon([(x-w/2+0.35, y-h/2), (x+w/2, y-h/2),
                                 (x+w/2-0.35, y+h/2), (x-w/2, y+h/2)],
                linewidth=1.4, edgecolor="#383", facecolor="#e6ffe6")
        ax.add_patch(p)
        ax.text(x, y, text, ha="center", va="center", fontsize=10.5)

    def arrow(x1, y1, x2, y2):
        ax.annotate("", xy=(x2, y2), xytext=(x1, y1),
            arrowprops=dict(arrowstyle="->", lw=1.3, color="black"))

    def label(x, y, text):
        ax.text(x, y, text, fontsize=10, color="#333",
                ha="center", va="center",
                bbox=dict(boxstyle="round,pad=0.15",
                          facecolor="white", edgecolor="none"))

    # --- Вертикальна послідовність блоків ---
    # (y_center, width, height, text, shape)
    nodes = [
        (38.8, 3.2, 1.2, "Початок",                                              "oval"),
        (36.6, 7.0, 1.2, "MPI_Init; отримати world_rank,\nworld_size = P",        "rect"),
        (34.1, 7.4, 1.6, "Перевірка:\nP = q²  та  N mod q = 0",                   "diamond"),
        (31.4, 7.4, 1.2, "MPI_Cart_create: решітка q × q,\nperiods = (0, 1)",     "rect"),
        (29.1, 7.4, 1.2, "MPI_Cart_sub: row_comm, col_comm",                      "rect"),
        (26.8, 7.8, 1.2, "Формування типу block_type\n(MPI_Type_vector) і block_resized", "rect"),
        (24.3, 8.0, 1.3, "MPI_Scatterv: розсилання блоків\nA_{i,j}, B_{i,j} у процеси (i,j)", "par"),
        (22.0, 3.8, 1.0, "step ← 0",                                              "rect"),
        (19.6, 5.0, 1.3, "step < q ?",                                            "diamond"),
        (16.7, 8.4, 1.4, "pivot ← (row + step) mod q;\n"
                         "копіювати A_loc у A_buf, якщо col = pivot",             "rect"),
        (14.1, 8.4, 1.3, "MPI_Bcast(A_buf, b·b) у row_comm\nвід процесу pivot",   "par"),
        (11.6, 7.6, 1.2, "C_loc ← C_loc + A_buf · B_loc\n(локальне множення)",    "rect"),
        (9.0,  8.4, 1.3, "MPI_Sendrecv: циклічний зсув\nB_loc вгору по col_comm", "par"),
        (6.6,  4.6, 1.0, "step ← step + 1",                                       "rect"),
        # ↓ нижче діамант виходу «ні» зведе сюди
        (3.8,  7.8, 1.2, "MPI_Gatherv: збирання C у корінь", "par"),
        (1.8,  6.0, 1.0, "Вимір max|ΔC|; вивід метрик;\nMPI_Finalize", "rect"),
        (0.3,  2.6, 1.0, "Кінець", "oval"),
    ]
    # Малюємо блоки
    for y, w, h, text, shape in nodes:
        box(CENTER, y, w, h, text, shape=shape)

    # Стрілки «згори-вниз» між сусідніми блоками
    def y_top(i):    return nodes[i][0] + nodes[i][2] / 2
    def y_bot(i):    return nodes[i][0] - nodes[i][2] / 2

    # Від 0 до 13 йдемо послідовно
    for i in range(len(nodes) - 1):
        # пропускаємо ділянки, які малюватимемо вручну (діаманти, цикл назад)
        if i == 2:       # після перевірки — «так» донизу
            arrow(CENTER, y_bot(2), CENTER, y_top(3))
            label(CENTER + 0.8, (y_bot(2) + y_top(3)) / 2, "так")
            continue
        if i == 7:       # step←0 → цикл
            arrow(CENTER, y_bot(7), CENTER, y_top(8))
            continue
        if i == 8:       # цикл → тіло (так)
            arrow(CENTER, y_bot(8), CENTER, y_top(9))
            label(CENTER + 0.9, (y_bot(8) + y_top(9)) / 2, "так")
            continue
        if i == 13:      # step++ → повернення до діаманту (малюватимемо нижче)
            continue
        if i == 14:      # Gatherv → Finalize-блок
            arrow(CENTER, y_bot(14), CENTER, y_top(15))
            continue
        arrow(CENTER, y_bot(i), CENTER, y_top(i + 1))
    arrow(CENTER, y_bot(15), CENTER, y_top(16))

    # «ні» з перевірки (2) — праворуч у блок помилки
    y_check = nodes[2][0]
    ax.plot([CENTER + nodes[2][1] / 2, ERROR_X - 0.9],
            [y_check, y_check], color="black", lw=1.3)
    ax.annotate("", xy=(ERROR_X - 0.9, y_check),
                xytext=(ERROR_X - 1.1, y_check),
                arrowprops=dict(arrowstyle="->", lw=1.3, color="black"))
    label(CENTER + nodes[2][1] / 2 + 1.0, y_check + 0.5, "ні")
    box(ERROR_X, y_check, 2.0, 1.6,
        "MPI_Finalize\n(код 2 / 3)", color="#ffecec")

    # «ні» з діаманту циклу (8) — ліворуч і вниз до Gatherv
    y_loop = nodes[8][0]
    x_loop_left = CENTER - nodes[8][1] / 2
    y_gather = nodes[14][0]
    # горизонталь від діаманту до лівої смуги
    ax.plot([x_loop_left, LEFT_LANE], [y_loop, y_loop], color="black", lw=1.3)
    # вертикаль вниз до рівня Gatherv
    ax.plot([LEFT_LANE, LEFT_LANE], [y_loop, y_gather], color="black", lw=1.3)
    # горизонталь з лівої смуги у Gatherv
    ax.annotate("", xy=(CENTER - nodes[14][1] / 2, y_gather),
                xytext=(LEFT_LANE, y_gather),
                arrowprops=dict(arrowstyle="->", lw=1.3, color="black"))
    label(x_loop_left - 0.9, y_loop + 0.5, "ні")

    # Зворотний зв'язок циклу: step++ (13) -> діамант (8), через правий край
    y_inc = nodes[13][0]
    x_inc_right = CENTER + nodes[13][1] / 2
    RIGHT_LANE = 10.6
    ax.plot([x_inc_right, RIGHT_LANE], [y_inc, y_inc], color="black", lw=1.3)
    ax.plot([RIGHT_LANE, RIGHT_LANE], [y_inc, y_loop], color="black", lw=1.3)
    ax.annotate("", xy=(CENTER + nodes[8][1] / 2, y_loop),
                xytext=(RIGHT_LANE, y_loop),
                arrowprops=dict(arrowstyle="->", lw=1.3, color="black"))

    ax.text(CENTER, 0.8 - 1.0,
            "",  # підпис малюнка робиться окремо підписом фігури у DOCX
            ha="center", fontsize=11, fontweight="bold")
    plt.tight_layout()
    plt.savefig(OUT / "fox_flowchart.png", dpi=150, bbox_inches="tight")
    plt.close()

# ------------------ 2. Схема структури програми (модулі) -----------------------

def program_structure():
    fig, ax = plt.subplots(figsize=(10.5, 7.0))
    ax.set_xlim(0, 14); ax.set_ylim(0, 10)
    ax.axis("off")

    def mod(x, y, w, h, title, body, color="#eef4ff", edge="#2244aa"):
        p = patches.FancyBboxPatch((x, y), w, h,
            boxstyle="round,pad=0.15", linewidth=1.4,
            edgecolor=edge, facecolor=color)
        ax.add_patch(p)
        ax.text(x + w/2, y + h - 0.35, title, ha="center", va="top",
                fontsize=11, fontweight="bold")
        ax.text(x + 0.2, y + h - 0.9, body, ha="left", va="top", fontsize=9)

    def arrow(p1, p2):
        ax.annotate("", xy=p2, xytext=p1,
            arrowprops=dict(arrowstyle="->", lw=1.3, color="#333"))

    mod(0.3, 7.8, 4.1, 1.9, "main()",
        "• parse CLI\n• MPI_Init\n• виклик fox_run(...)\n• MPI_Finalize")
    mod(4.8, 7.8, 4.1, 1.9, "fox_run()",
        "• Cart_create q×q\n• row/col comm\n• scatter A, B\n• головний цикл Fox\n• gather C")
    mod(9.3, 7.8, 4.1, 1.9, "Перевірка",
        "• max|ΔC| з еталоном\n• checksum\n• вивід у stdout/лог")

    mod(0.3, 5.0, 4.1, 2.2, "Послідовний еталон\nfox_seq.cpp",
        "• fill_matrix(seed)\n• triple-loop gemm\n• запис у ref_<N>.bin",
        color="#fff6e6", edge="#a37")
    mod(4.8, 5.0, 4.1, 2.2, "Паралельний\nfox_mpi.cpp",
        "• local_gemm_add\n• MPI_Type_vector\n• Scatterv/Gatherv\n• Bcast, Sendrecv",
        color="#e6ffe6", edge="#383")
    mod(9.3, 5.0, 4.1, 2.2, "Тести\nfox_tests.cpp",
        "• I·B = B\n• 0·B = 0\n• малі матриці\n• випадкові",
        color="#f9e6ff", edge="#63a")

    mod(0.3, 2.2, 4.1, 2.2, "run_experiments.sh\nextra_experiments.sh",
        "• seq для T1\n• mpi для кожного P\n• summary.csv",
        color="#ffeae6", edge="#c33")
    mod(4.8, 2.2, 4.1, 2.2, "make_charts.py",
        "• graphs time/S/E\n• 3D surfaces\n• time_breakdown",
        color="#e6eefa", edge="#336")
    mod(9.3, 2.2, 4.1, 2.2, "make_diagrams.py",
        "• блок-схема\n• структура програми\n• схеми Фокса",
        color="#e6faea", edge="#262")

    mod(0.3, 0.2, 13.1, 1.6, "Вихідні артефакти",
        "results/summary.csv  |  charts/*.png  |  charts3d/*.png  |  "
        "diagrams/*.png  |  screenshots/*.png  |  report/*.docx / *.pdf",
        color="#f5f5f5", edge="#555")

    # Стрілки потоку даних
    arrow((4.4, 8.7), (4.8, 8.7))
    arrow((8.9, 8.7), (9.3, 8.7))
    arrow((2.4, 7.8), (2.4, 7.2))  # main -> seq
    arrow((6.85, 7.8), (6.85, 7.2))
    arrow((11.35, 7.8), (11.35, 7.2))
    arrow((2.4, 5.0), (2.4, 4.4))
    arrow((6.85, 5.0), (6.85, 4.4))
    arrow((11.35, 5.0), (11.35, 4.4))

    ax.text(7, -0.2, "Рисунок. Структура програмної реалізації",
        ha="center", fontsize=11, fontweight="bold")
    plt.tight_layout()
    plt.savefig(OUT / "program_structure.png", dpi=150, bbox_inches="tight")
    plt.close()

# --------- 3. Ілюстрація ітерацій Фокса (блочний розподіл) ---------------------

def fox_grid_illustration():
    q = 3
    fig, axes = plt.subplots(1, q, figsize=(11.0, 4.0))
    colors_A = plt.cm.Blues(np.linspace(0.35, 0.9, q*q))
    colors_B = plt.cm.Greens(np.linspace(0.35, 0.9, q*q))

    for step_idx in range(q):
        ax = axes[step_idx]
        ax.set_xlim(-0.2, q + 0.4)
        ax.set_ylim(-0.2, q + 0.4)
        ax.set_aspect("equal")
        ax.axis("off")
        ax.set_title(f"Крок {step_idx + 1}", fontsize=11)

        for i in range(q):
            for j in range(q):
                rect = patches.Rectangle((j, q - 1 - i), 1, 1,
                    linewidth=1.0, edgecolor="#333", facecolor="#f8f8f8")
                ax.add_patch(rect)
                pivot_col = (i + step_idx) % q
                is_pivot = (j == pivot_col)
                if is_pivot:
                    a = patches.Rectangle((j + 0.05, q - 1 - i + 0.55), 0.35, 0.35,
                        facecolor=colors_A[i*q + pivot_col],
                        edgecolor="#113", linewidth=0.8)
                    ax.add_patch(a)
                    ax.text(j + 0.22, q - 1 - i + 0.72,
                        f"A{i}{pivot_col}", fontsize=8, ha="center", va="center",
                        color="white")
                # B shifted up by step_idx on column j
                b_row = (i + step_idx) % q
                b_rect = patches.Rectangle((j + 0.55, q - 1 - i + 0.05), 0.35, 0.35,
                    facecolor=colors_B[b_row*q + j],
                    edgecolor="#131", linewidth=0.8)
                ax.add_patch(b_rect)
                ax.text(j + 0.72, q - 1 - i + 0.22,
                    f"B{b_row}{j}", fontsize=8, ha="center", va="center",
                    color="white")
    fig.suptitle("Іллюстрація алгоритму Фокса (q=3): broadcast блока A та зсув B",
                 fontsize=12)
    plt.tight_layout()
    plt.savefig(OUT / "fox_iterations.png", dpi=150, bbox_inches="tight")
    plt.close()

# --------- 4. Схема декартової решітки q×q процесів ----------------------------

def grid_scheme():
    q = 4
    fig, ax = plt.subplots(figsize=(7.2, 7.2))
    ax.set_xlim(-0.5, q + 0.5)
    ax.set_ylim(-0.5, q + 1.0)
    ax.set_aspect("equal")
    ax.axis("off")

    for i in range(q):
        for j in range(q):
            rect = patches.FancyBboxPatch((j, q - 1 - i), 0.9, 0.9,
                boxstyle="round,pad=0.08", linewidth=1.2,
                edgecolor="#224", facecolor="#eaf3ff")
            ax.add_patch(rect)
            rank = i * q + j
            ax.text(j + 0.45, q - 1 - i + 0.58,
                f"P{rank}", ha="center", va="center", fontsize=11, fontweight="bold")
            ax.text(j + 0.45, q - 1 - i + 0.28,
                f"({i},{j})", ha="center", va="center", fontsize=9)

    for i in range(q):
        ax.annotate("", xy=(q + 0.3, q - 1 - i + 0.45),
                    xytext=(-0.3, q - 1 - i + 0.45),
                    arrowprops=dict(arrowstyle="-|>", lw=1.0, color="#a33",
                                    alpha=0.5))
    for j in range(q):
        ax.annotate("", xy=(j + 0.45, -0.3),
                    xytext=(j + 0.45, q + 0.3),
                    arrowprops=dict(arrowstyle="-|>", lw=1.0, color="#383",
                                    alpha=0.5))
    ax.text(q + 0.35, q - 0.3, "row_comm", color="#a33", fontsize=10,
            ha="left", va="center")
    ax.text(0.1, -0.45, "col_comm", color="#383", fontsize=10,
            ha="left", va="center")
    ax.text(q/2, q + 0.6,
        f"Декартова решітка процесів q × q (q = {q}, P = {q*q})",
        ha="center", fontsize=12, fontweight="bold")
    plt.tight_layout()
    plt.savefig(OUT / "process_grid.png", dpi=150, bbox_inches="tight")
    plt.close()

flowchart_fox()
program_structure()
fox_grid_illustration()
grid_scheme()
print("Diagrams written to", OUT)
