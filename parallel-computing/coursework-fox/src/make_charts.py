#!/usr/bin/env python3
# Побудова графіків та тривимірних поверхонь за результатами експериментів.
# Читає results/summary.csv, формує PNG у charts/ та charts3d/.

import os
import csv
import math
from pathlib import Path

import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D  # noqa

ROOT = Path(__file__).resolve().parent.parent
RES  = ROOT / "results"
CH   = ROOT / "charts"
CH3  = ROOT / "charts3d"
CH.mkdir(exist_ok=True)
CH3.mkdir(exist_ok=True)

rows = []
with (RES / "summary.csv").open() as f:
    reader = csv.DictReader(f)
    for r in reader:
        rows.append({
            "N":  int(r["N"]),
            "P":  int(r["P"]),
            "q":  int(r["q"]),
            "T1": float(r["T1"]),
            "Tp": float(r["Tp"]),
            "S":  float(r["S"]),
            "E":  float(r["E"]),
            "md": float(r["max_diff"]),
        })

Ns = sorted({r["N"] for r in rows})
Ps = sorted({r["P"] for r in rows})

def get(N, P, key):
    for r in rows:
        if r["N"] == N and r["P"] == P:
            return r[key]
    return None

plt.rcParams.update({
    "font.family": "DejaVu Serif",
    "font.size": 11,
    "axes.grid": True,
    "grid.alpha": 0.35,
    "lines.linewidth": 2.0,
    "lines.markersize": 7,
})

# ---------- 2D-графіки ----------

# 1) Час виконання Tp від P для різних N
plt.figure(figsize=(7.2, 4.8))
for N in Ns:
    ys = [get(N, P, "Tp") for P in Ps]
    if None in ys: continue
    plt.plot(Ps, ys, marker="o", label=f"N = {N}")
plt.xscale("log", base=2)
plt.xticks(Ps, [str(p) for p in Ps])
plt.xlabel("Число процесів P")
plt.ylabel("Час виконання $T_p$, с")
plt.title("Залежність часу виконання від P")
plt.legend()
plt.tight_layout()
plt.savefig(CH / "time_vs_P.png", dpi=150)
plt.close()

# 2) Прискорення S від P
plt.figure(figsize=(7.2, 4.8))
for N in Ns:
    ys = [get(N, P, "S") for P in Ps]
    if None in ys: continue
    plt.plot(Ps, ys, marker="s", label=f"N = {N}")
plt.plot(Ps, Ps, "--", color="gray", label="ідеальне S = P")
plt.xlabel("Число процесів P")
plt.ylabel("Прискорення $S = T_1 / T_p$")
plt.title("Прискорення алгоритму Фокса")
plt.legend()
plt.tight_layout()
plt.savefig(CH / "speedup_vs_P.png", dpi=150)
plt.close()

# 3) Ефективність E від P
plt.figure(figsize=(7.2, 4.8))
for N in Ns:
    ys = [get(N, P, "E") for P in Ps]
    if None in ys: continue
    plt.plot(Ps, ys, marker="^", label=f"N = {N}")
plt.axhline(1.0, linestyle="--", color="gray", label="ідеальне E = 1")
plt.xlabel("Число процесів P")
plt.ylabel("Ефективність $E = S / P$")
plt.title("Ефективність паралелізації")
plt.legend()
plt.tight_layout()
plt.savefig(CH / "efficiency_vs_P.png", dpi=150)
plt.close()

# 4) Час від N при фіксованому P
plt.figure(figsize=(7.2, 4.8))
for P in Ps:
    ys = [get(N, P, "Tp") for N in Ns]
    if any(y is None for y in ys): continue
    plt.plot(Ns, ys, marker="o", label=f"P = {P}")
plt.xlabel("Розмір матриці N")
plt.ylabel("Час $T_p$, с")
plt.title("Залежність часу від розміру задачі")
plt.legend()
plt.tight_layout()
plt.savefig(CH / "time_vs_N.png", dpi=150)
plt.close()

# 5) Розбиття Tp на компоненти для P=9 (як приклад)
# compute/scatter/gather з mpi_runs.csv
mpi_rows = []
with (RES / "mpi_runs.csv").open() as f:
    for line in f:
        parts = line.strip().split(",")
        if len(parts) < 9: continue
        try:
            mpi_rows.append({
                "N": int(parts[0]), "P": int(parts[1]), "q": int(parts[2]),
                "Ttotal": float(parts[3]), "Tcomp": float(parts[4]),
                "Tsc": float(parts[5]), "Tga": float(parts[6]),
            })
        except ValueError:
            continue

for target_P in [4, 9]:
    xs = []
    comp, sc, ga = [], [], []
    for N in Ns:
        for r in mpi_rows:
            if r["N"] == N and r["P"] == target_P:
                xs.append(N)
                comp.append(r["Tcomp"])
                sc.append(r["Tsc"])
                ga.append(r["Tga"])
                break
    if not xs: continue
    fig, ax = plt.subplots(figsize=(7.2, 4.8))
    ax.bar(xs, comp, width=40, label="Обчислення", color="#4c78a8")
    ax.bar(xs, sc, width=40, bottom=comp, label="Scatter", color="#f58518")
    ax.bar(xs, ga, width=40, bottom=[c+s for c,s in zip(comp,sc)],
           label="Gather", color="#54a24b")
    ax.set_xlabel("Розмір матриці N")
    ax.set_ylabel("Час, с")
    ax.set_title(f"Структура часу виконання при P = {target_P}")
    ax.legend()
    plt.tight_layout()
    plt.savefig(CH / f"time_breakdown_P{target_P}.png", dpi=150)
    plt.close()

# ---------- 3D-графіки ----------

# Сітка (N, P) -> метрика. Пропуски (де немає запуску) заповнимо NaN.
N_arr = np.array(Ns, dtype=float)
P_arr = np.array(Ps, dtype=float)
NN, PP = np.meshgrid(N_arr, P_arr, indexing="ij")

def build_grid(key):
    Z = np.full_like(NN, np.nan, dtype=float)
    for i, N in enumerate(Ns):
        for j, P in enumerate(Ps):
            v = get(N, P, key)
            if v is not None:
                Z[i, j] = v
    return Z

def plot3d(key, zlabel, title, fname, cmap="viridis"):
    Z = build_grid(key)
    fig = plt.figure(figsize=(8.4, 5.6))
    ax = fig.add_subplot(111, projection="3d")
    surf = ax.plot_surface(NN, PP, Z, cmap=cm.get_cmap(cmap),
                           linewidth=0.2, edgecolor="k", antialiased=True)
    ax.set_xlabel("N")
    ax.set_ylabel("P")
    ax.set_zlabel(zlabel)
    ax.set_yticks(Ps)
    ax.set_title(title)
    fig.colorbar(surf, shrink=0.65, aspect=14, pad=0.08)
    plt.tight_layout()
    plt.savefig(CH3 / fname, dpi=150)
    plt.close()

plot3d("Tp", "T_p, с", "3D: час виконання $T_p(N, P)$",
       "surface_time.png", "plasma")
plot3d("S",  "S", "3D: прискорення $S(N, P)$",
       "surface_speedup.png", "viridis")
plot3d("E",  "E", "3D: ефективність $E(N, P)$",
       "surface_efficiency.png", "cividis")

print("Charts written to", CH)
print("3D charts written to", CH3)
