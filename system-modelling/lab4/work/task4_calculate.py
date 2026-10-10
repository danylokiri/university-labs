"""Розрахунок усіх критеріїв якості/ефективності для T1 і T2 + згорток."""
import os
import json
import sys
import io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

H_DT = 60  # диктуючий тиск, м

# Дані з scheme.png
NODES = [1, 2, 3, 4, 5, 6, 7]  # включно з вузлом активного джерела
H_T1 = {1: 65, 2: 64, 3: 61, 4: 58, 5: 61, 6: 54, 7: 58}
H_T2 = {1: 66, 2: 63, 3: 61, 4: 59, 5: 62, 6: 56, 7: 59}
Q1_T1 = 2700
Q1_T2 = 2800


def deltas(H):
    return {i: H[i] - H_DT for i in NODES}


def deficit_set(D):
    return {i: -D[i] for i in D if D[i] < 0}  # |ΔH|


def excess_set(D):
    return {i: D[i] for i in D if D[i] > 0}


def momentary_criteria(H, Q1):
    D = deltas(H)
    deficits = deficit_set(D)
    excess = excess_set(D)
    N = len(NODES)
    K1 = len(deficits) / N if N else 0.0
    K2 = sum(deficits.values())
    K3 = K2 / N if N else 0.0
    K4 = max(deficits.values()) if deficits else 0.0
    K5 = H[1] * Q1
    K6 = sum(excess.values())
    K7 = max(excess.values()) if excess else 0.0
    return {
        "delta": D,
        "deficits": deficits,
        "excess": excess,
        "K1": K1,    # ймовірнісний (якість)
        "K2": K2,    # сума дефіцитів (якість)
        "K3": K3,    # приведений сум. дефіцит (якість)
        "K4": K4,    # max глибина дефіциту (якість)
        "K5": K5,    # приведені енерговитрати (ефективність)
        "K6": K6,    # сума надлишків (ефективність)
        "K7": K7,    # max надлишок (ефективність)
    }


def fmt(num, digits=4):
    if isinstance(num, (int, float)):
        if isinstance(num, float):
            return f"{num:.{digits}f}".rstrip("0").rstrip(".")
        return str(num)
    return str(num)


def main():
    R1 = momentary_criteria(H_T1, Q1_T1)
    R2 = momentary_criteria(H_T2, Q1_T2)

    convolutions = {
        "C1": (R1["K1"] + R2["K1"]) / 2,
        "C2": (R1["K2"] + R2["K2"]) / 2,
        "C3": (R1["K3"] + R2["K3"]) / 2,
        "C4": max(R1["K4"], R2["K4"]),
        "C5": R1["K5"] + R2["K5"],
        "C6": (R1["K6"] + R2["K6"]) / 2,
        "C7": max(R1["K7"], R2["K7"]),
    }

    out = {
        "Hd_t": H_DT,
        "T1": {"H": H_T1, "Q1": Q1_T1, **R1},
        "T2": {"H": H_T2, "Q1": Q1_T2, **R2},
        "convolutions": convolutions,
    }

    os.makedirs("lab4/work", exist_ok=True)
    with open("lab4/work/task4_results.json", "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=2, default=lambda x: dict(x) if hasattr(x, 'items') else x)

    # Pretty-print
    print("=" * 80)
    print("МОМЕНТ T1")
    print("=" * 80)
    print(f"  Hi: {H_T1}")
    print(f"  Q1 = {Q1_T1}")
    print(f"  ΔHi = Hi - {H_DT} (м):")
    for i in NODES:
        d = R1["delta"][i]
        sign = "(надл.)" if d > 0 else ("(деф.)" if d < 0 else "(допуст.)")
        print(f"    Δ{i} = {d:+d} {sign}")
    print(f"  Дефіцитні (|ΔH|): {R1['deficits']}")
    print(f"  Надлишкові (ΔH): {R1['excess']}")
    print()
    print("  Критерії якості:")
    print(f"    K1 (N_деф/N)        = {R1['K1']:.6f}  ({len(R1['deficits'])}/{len(NODES)})")
    print(f"    K2 (Σ|ΔH деф|, м)   = {R1['K2']}")
    print(f"    K3 (K2/N, м)        = {R1['K3']:.6f}")
    print(f"    K4 (max|ΔH деф|, м) = {R1['K4']}")
    print()
    print("  Критерії ефективності:")
    print(f"    K5 (H1·Q1)          = {R1['K5']}")
    print(f"    K6 (Σ ΔH надл, м)   = {R1['K6']}")
    print(f"    K7 (max ΔH надл, м) = {R1['K7']}")

    print("\n" + "=" * 80)
    print("МОМЕНТ T2")
    print("=" * 80)
    print(f"  Hi: {H_T2}")
    print(f"  Q1 = {Q1_T2}")
    print(f"  ΔHi (м):")
    for i in NODES:
        d = R2["delta"][i]
        sign = "(надл.)" if d > 0 else ("(деф.)" if d < 0 else "(допуст.)")
        print(f"    Δ{i} = {d:+d} {sign}")
    print(f"  Дефіцитні: {R2['deficits']}")
    print(f"  Надлишкові: {R2['excess']}")
    print()
    print("  Критерії якості:")
    print(f"    K1 = {R2['K1']:.6f}  ({len(R2['deficits'])}/{len(NODES)})")
    print(f"    K2 = {R2['K2']}")
    print(f"    K3 = {R2['K3']:.6f}")
    print(f"    K4 = {R2['K4']}")
    print()
    print("  Критерії ефективності:")
    print(f"    K5 = {R2['K5']}")
    print(f"    K6 = {R2['K6']}")
    print(f"    K7 = {R2['K7']}")

    print("\n" + "=" * 80)
    print("ЗГОРТКИ ЗА ІНТЕРВАЛ [T1, T2]")
    print("=" * 80)
    print(f"  C1 (середн. ймовірн. дефіциту)        = {convolutions['C1']:.6f}")
    print(f"  C2 (середн. сума дефіциту, м)         = {convolutions['C2']}")
    print(f"  C3 (середн. приведений дефіцит, м)    = {convolutions['C3']:.6f}")
    print(f"  C4 (max глибина дефіциту, м)          = {convolutions['C4']}")
    print(f"  C5 (сумарні енерговитрати)            = {convolutions['C5']}")
    print(f"  C6 (середня сума надлишків, м)        = {convolutions['C6']}")
    print(f"  C7 (max надлишок, м)                  = {convolutions['C7']}")


if __name__ == "__main__":
    main()
