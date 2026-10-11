import csv, os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.makedirs("charts", exist_ok=True)
ns, tb, ts, tq = [], [], [], []
with open("results/sequential_times.csv") as f:
    r = csv.DictReader(f)
    for row in r:
        ns.append(int(row["n"]))
        tb.append(float(row["bubble_s"]))
        ts.append(float(row["shell_s"]))
        tq.append(float(row["quick_s"]))

plt.figure(figsize=(8,5))
plt.plot(ns, tb, "o-", label="Бульбашкове")
plt.plot(ns, ts, "s-", label="Сортування Шелла")
plt.plot(ns, tq, "^-", label="Швидке сортування")
plt.xlabel("Розмір масиву n")
plt.ylabel("Час виконання, с")
plt.title("Залежність часу сортування від розміру масиву")
plt.grid(True, alpha=0.4)
plt.legend()
plt.tight_layout()
plt.savefig("charts/time_vs_n_linear.png", dpi=130)
plt.close()

plt.figure(figsize=(8,5))
plt.loglog(ns, tb, "o-", label="Бульбашкове  O(n²)")
plt.loglog(ns, ts, "s-", label="Шелла  ~O(n^1.3)")
plt.loglog(ns, tq, "^-", label="Швидке  O(n log n)")
plt.xlabel("Розмір масиву n (log)")
plt.ylabel("Час, с (log)")
plt.title("Залежність часу сортування від n (логарифмічна шкала)")
plt.grid(True, which="both", alpha=0.4)
plt.legend()
plt.tight_layout()
plt.savefig("charts/time_vs_n_log.png", dpi=130)
plt.close()

plt.figure(figsize=(8,5))
plt.plot(ns, ts, "s-", label="Сортування Шелла")
plt.plot(ns, tq, "^-", label="Швидке сортування")
plt.xlabel("Розмір масиву n")
plt.ylabel("Час, с")
plt.title("Шелла vs швидке сортування (без бульбашкового)")
plt.grid(True, alpha=0.4)
plt.legend()
plt.tight_layout()
plt.savefig("charts/time_shell_quick.png", dpi=130)
plt.close()
print("OK")
