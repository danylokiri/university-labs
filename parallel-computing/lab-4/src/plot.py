import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.makedirs("charts", exist_ok=True)

data = {}   # alg -> {N -> {P -> (seq,par,S,E)}}
for r in csv.DictReader(open("results/matmul.csv")):
    a = r["alg"]; N = int(r["N"]); P = int(r["P"])
    data.setdefault(a, {}).setdefault(N, {})[P] = (
        float(r["seq_t"]), float(r["par_t"]),
        float(r["S"]), float(r["E"]))

names = {"ribbon": "стрічковий", "fox": "Фокса", "cannon": "Кенона"}
prefix = {"ribbon": "ribbon", "fox": "fox", "cannon": "cannon"}

for alg, by in data.items():
    Ns = sorted(by.keys()); pf = prefix[alg]; nm = names[alg]

    plt.figure(figsize=(8,5))
    for N in Ns:
        Ps = sorted(by[N].keys())
        ys = [by[N][p][1] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.xlabel("P"); plt.ylabel("Tp, с"); plt.title(f"Час — {nm}")
    plt.grid(True, alpha=0.4); plt.legend(); plt.tight_layout()
    plt.savefig(f"charts/{pf}_time.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for N in Ns:
        Ps = sorted(by[N].keys())
        ys = [by[N][p][2] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    Pmax = max(p for d in by.values() for p in d)
    plt.plot(range(1, Pmax+1), range(1, Pmax+1), "k--", alpha=0.5, label="Ідеал")
    plt.xlabel("P"); plt.ylabel("S"); plt.title(f"Прискорення — {nm}")
    plt.grid(True, alpha=0.4); plt.legend(); plt.tight_layout()
    plt.savefig(f"charts/{pf}_speedup.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for N in Ns:
        Ps = sorted(by[N].keys())
        ys = [by[N][p][3] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"N={N}")
    plt.axhline(1.0, color="k", linestyle="--", alpha=0.5)
    plt.xlabel("P"); plt.ylabel("E"); plt.title(f"Ефективність — {nm}")
    plt.grid(True, alpha=0.4); plt.legend(); plt.tight_layout()
    plt.savefig(f"charts/{pf}_efficiency.png", dpi=130); plt.close()

# Порівняння трьох при P=4
plt.figure(figsize=(8,5))
for alg in ["ribbon", "fox", "cannon"]:
    by = data[alg]; Ns = sorted(by.keys())
    ys = []
    for N in Ns:
        ys.append(by[N].get(4, by[N][1])[1])
    plt.plot(Ns, ys, "o-", label=names[alg])
plt.xlabel("N"); plt.ylabel("Tp, с (P=4)")
plt.title("Порівняння трьох алгоритмів при P=4")
plt.grid(True, alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/compare_p4.png", dpi=130); plt.close()
print("OK")
