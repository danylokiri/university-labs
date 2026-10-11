import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.makedirs("charts", exist_ok=True)

def load(path):
    by = {}
    for r in csv.DictReader(open(path)):
        N = int(r["N"]); P = int(r["P"])
        by.setdefault(N, {})[P] = (
            float(r["seq_t"]), float(r["par_t"]),
            float(r["S"]), float(r["E"]))
    return by

def plot_one(by, prefix, title):
    Ns = sorted(by.keys())
    Ps = sorted({p for v in by.values() for p in v})

    plt.figure(figsize=(8,5))
    for N in Ns:
        ys = [by[N][p][1] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.xlabel("Кількість процесів P")
    plt.ylabel("Час Tp, с")
    plt.title(f"Час паралельного виконання — {title}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout()
    plt.savefig(f"charts/{prefix}_time.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for N in Ns:
        ys = [by[N][p][2] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.plot(Ps, Ps, "k--", alpha=0.5, label="Ідеал S=p")
    plt.xlabel("Кількість процесів P")
    plt.ylabel("Прискорення S")
    plt.title(f"Прискорення — {title}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout()
    plt.savefig(f"charts/{prefix}_speedup.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for N in Ns:
        ys = [by[N][p][3] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.axhline(1.0, color="k", linestyle="--", alpha=0.5, label="Ідеал E=1")
    plt.xlabel("Кількість процесів P")
    plt.ylabel("Ефективність E")
    plt.title(f"Ефективність — {title}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout()
    plt.savefig(f"charts/{prefix}_efficiency.png", dpi=130); plt.close()

plot_one(load("results/minmax.csv"),  "minmax",  "пошук max/min")
plot_one(load("results/average.csv"), "average", "середнє значення")
print("OK")
