import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.makedirs("charts", exist_ok=True)
data = {}
for r in csv.DictReader(open("results/integral.csv")):
    m = int(r["method"]); n = int(r["n"]); P = int(r["P"])
    data.setdefault(m, {}).setdefault(n, {})[P] = (
        float(r["seq_t"]), float(r["par_t"]),
        float(r["S"]), float(r["E"]), float(r["err"]))

names = {0: "трапецій", 1: "Сімпсона"}
prefix = {0: "trap", 1: "simp"}

for m, by in data.items():
    Ns = sorted(by.keys()); Ps = sorted({p for v in by.values() for p in v})
    pf = prefix[m]; nm = names[m]

    plt.figure(figsize=(8,5))
    for n in Ns:
        ys = [by[n][p][1] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"n={n}")
    plt.xlabel("P"); plt.ylabel("Tp, с")
    plt.title(f"Час паралельного методу {nm}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout(); plt.savefig(f"charts/{pf}_time.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for n in Ns:
        ys = [by[n][p][2] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"n={n}")
    plt.plot(Ps, Ps, "k--", alpha=0.5, label="Ідеал S=p")
    plt.xlabel("P"); plt.ylabel("S")
    plt.title(f"Прискорення — метод {nm}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout(); plt.savefig(f"charts/{pf}_speedup.png", dpi=130); plt.close()

    plt.figure(figsize=(8,5))
    for n in Ns:
        ys = [by[n][p][3] for p in Ps]
        plt.plot(Ps, ys, "o-", label=f"n={n}")
    plt.axhline(1.0, color="k", linestyle="--", alpha=0.5)
    plt.xlabel("P"); plt.ylabel("E")
    plt.title(f"Ефективність — метод {nm}")
    plt.grid(True, alpha=0.4); plt.legend()
    plt.tight_layout(); plt.savefig(f"charts/{pf}_efficiency.png", dpi=130); plt.close()

# Похибка vs n при P=8
plt.figure(figsize=(8,5))
for m in [0, 1]:
    Ns = sorted(data[m].keys())
    ys = [data[m][n][8][4] for n in Ns]
    plt.loglog(Ns, ys, "o-", label=f"метод {names[m]}")
plt.xlabel("n (log)"); plt.ylabel("|I − Iexact| (log)")
plt.title("Похибка чисельного інтегрування (P=8)")
plt.grid(True, which="both", alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/error_vs_n.png", dpi=130); plt.close()

# Порівняння методів при P=4
plt.figure(figsize=(8,5))
for m in [0, 1]:
    Ns = sorted(data[m].keys())
    ys = [data[m][n][4][1] for n in Ns]
    plt.plot(Ns, ys, "o-", label=names[m])
plt.xlabel("n"); plt.ylabel("Tp, с (P=4)")
plt.title("Порівняння методів інтегрування при P=4")
plt.grid(True, alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/compare_p4.png", dpi=130); plt.close()
print("OK")
