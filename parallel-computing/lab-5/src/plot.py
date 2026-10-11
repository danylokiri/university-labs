import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.makedirs("charts", exist_ok=True)
data = {}
for r in csv.DictReader(open("results/gauss.csv")):
    N = int(r["N"]); P = int(r["P"])
    data.setdefault(N, {})[P] = (float(r["seq_t"]), float(r["par_t"]),
                                  float(r["S"]), float(r["E"]))

Ns = sorted(data.keys())
Ps = sorted({p for v in data.values() for p in v})

plt.figure(figsize=(8,5))
for N in Ns:
    ys = [data[N][p][1] for p in Ps]
    plt.plot(Ps, ys, "o-", label=f"N={N}")
plt.xlabel("P"); plt.ylabel("Tp, с")
plt.title("Час паралельного методу Гауса")
plt.grid(True, alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/gauss_time.png", dpi=130); plt.close()

plt.figure(figsize=(8,5))
for N in Ns:
    ys = [data[N][p][2] for p in Ps]
    plt.plot(Ps, ys, "o-", label=f"N={N}")
plt.plot(Ps, Ps, "k--", alpha=0.5, label="Ідеал S=p")
plt.xlabel("P"); plt.ylabel("S")
plt.title("Прискорення")
plt.grid(True, alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/gauss_speedup.png", dpi=130); plt.close()

plt.figure(figsize=(8,5))
for N in Ns:
    ys = [data[N][p][3] for p in Ps]
    plt.plot(Ps, ys, "o-", label=f"N={N}")
plt.axhline(1.0, color="k", linestyle="--", alpha=0.5)
plt.xlabel("P"); plt.ylabel("E")
plt.title("Ефективність")
plt.grid(True, alpha=0.4); plt.legend()
plt.tight_layout(); plt.savefig("charts/gauss_efficiency.png", dpi=130); plt.close()
print("OK")
