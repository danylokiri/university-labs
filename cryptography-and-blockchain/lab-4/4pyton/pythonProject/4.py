import numpy as np
import matplotlib.pyplot as plt

def attacker_success_probability(q, p, z):
    lambda_ = z * (q / p)
    sum_ = 0.0

    for k in range(z + 1):
        poisson = poisson_probability(lambda_, k)
        term = poisson * (1 - (q / p) ** (z - k))
        sum_ += term

    return 1 - sum_

def poisson_probability(lambda_, k):
    poisson = np.exp(-lambda_)
    for i in range(1, k + 1):
        poisson *= lambda_ / i
    return poisson

def calculate_minimum_blocks(q, threshold):
    p = 1.0 - q
    z = 0

    while True:
        probability = attacker_success_probability(q, p, z)
        if probability < threshold:
            return z
        z += 1

def main():
    thresholds = [1e-3, 1e-4, 1e-5]
    q_values = np.arange(0.1, 0.45 + 0.05, 0.05)

    plt.figure(figsize=(10, 6))
    plt.xlabel("Частка зловмисників (q)")
    plt.ylabel("Кількість блоків підтвердження (z)")
    plt.title("Залежність кількості блоків підтвердження від частки зловмисників")

    for threshold in thresholds:
        z_values = []
        for q in q_values:
            z = calculate_minimum_blocks(q, threshold)
            print(f"Поріг: {threshold:.1e}, q = {q:.2f}, z = {z}")
            z_values.append(z)
        plt.plot(q_values, z_values, label=f'Поріг: {threshold:.1e}')

    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()
