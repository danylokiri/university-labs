import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.makedirs("charts", exist_ok=True)

rows = list(csv.DictReader(open("results/sort.csv")))
data = {}  # alg -> {N -> {P -> (seq,par,S,E)}}
for r in rows:
    alg = int(r["alg"]); N = int(r["N"]); P = int(r["P"])
    data.setdefault(alg, {}).setdefault(N, {})[P] = (
        float(r["seq_t"]), float(r["par_t"]),
        float(r["S"]), float(r["E"]))

names = {2: "швидке сортування", 3: "парно-непарне сортування"}
prefix = {2: "quick", 3: "oddeven"}

for alg, by in data.items():
    Ns = sorted(by.keys()); Ps = sorted({p for v in by.values() for p in v})
    nm = names[alg]; pf = prefix[alg]

    plt.figure(figsize=(8,5))
    for N in Ns:
        ys = [by[N][p][1] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.xlabel("P"); plt.ylabel("Час, с")
    plt.title(f"Час паралельного сортування — {nm}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout(); plt.savefig(f"charts/{pf}_time.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for N in Ns:
        ys = [by[N][p][2] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.plot(Ps, Ps, "k--", alpha=0.5, label="Ідеал S=p")
    plt.xlabel("P"); plt.ylabel("Прискорення S")
    plt.title(f"Прискорення — {nm}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout(); plt.savefig(f"charts/{pf}_speedup.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for N in Ns:
        ys = [by[N][p][3] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.axhline(1.0, color="k", linestyle="--", alpha=0.5)
    plt.xlabel("P"); plt.ylabel("Ефективність E")
    plt.title(f"Ефективність — {nm}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout(); plt.savefig(f"charts/{pf}_efficiency.png", dpi=130); plt.close()

# Порівняння алгоритмів при P=4
plt.figure(figsize=(8,5))
for alg, by in data.items():
    Ns = sorted(by.keys())
    ys = [by[N][4][1] for N in Ns]
    plt.plot(Ns, ys, "o-", label=names[alg])
plt.xlabel("N"); plt.ylabel("Tp, с (P=4)")
plt.title("Порівняння алгоритмів сортування при P=4")
plt.grid(True, alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/compare_p4.png", dpi=130); plt.close()
print("OK")
