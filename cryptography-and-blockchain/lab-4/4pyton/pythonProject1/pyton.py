import numpy as np
import matplotlib.pyplot as plt
from scipy.special import comb
import math

# Функция для вычисления вероятности успеха атаки двойной траты
# Используется формула (3) из задания с логарифмическим представлением биномного коэффициента

def probability_attack_success(z, p_m_prime, p_h_prime):
    if p_m_prime >= p_h_prime:
        return 1
    else:
        total_prob_log = -float('inf')  # Начальное значение для аккумулирования логарифмов
        for k in range(z + 1):
            # Логарифмическое представление биномного коэффициента для повышения точности
            log_comb = math.lgamma(z + 1) - math.lgamma(k + 1) - math.lgamma(z - k + 1)
            log_term = log_comb + (z - k) * math.log(p_m_prime) + k * math.log(p_h_prime)
            total_prob_log = np.logaddexp(total_prob_log, log_term)  # Используем logaddexp для аккуратного сложения экспонент
        return 1 - math.exp(total_prob_log)

# Параметры задачи
alpha_fixed = 0.00167
p_m_values = np.arange(0.1, 0.45, 0.05)
D_H_values = [0, 15, 30, 60, 120, 180]
alpha_values = [alpha_fixed, 1 / 600]  # Альфа фиксированное значение и альфа как 1/час генерации блока в секундах

# Основной цикл для перебора параметров и вычислений
for alpha in alpha_values:
    alpha_H_values = []
    alpha_M_values = []

    for p_m in p_m_values:
        p_h = 1 - p_m
        alpha_H = alpha * p_h
        alpha_M = alpha * p_m
        alpha_H_values.append(alpha_H)
        alpha_M_values.append(alpha_M)

    for D_H in D_H_values:
        blocks_confirmed = []

        for p_m, p_h, alpha_H, alpha_M in zip(p_m_values, [1 - p_m for p_m in p_m_values], alpha_H_values, alpha_M_values):
            p_m_prime = 1 - np.exp(-alpha_M * D_H) * p_h
            p_h_prime = np.exp(-alpha_M * D_H) * p_h

            # Добавление отладки для промежуточных значений
            print(f"alpha_M: {alpha_M}, D_H: {D_H}, p_m: {p_m}, p_m_prime: {p_m_prime}, p_h_prime: {p_h_prime}")

            z = 10  # Начальное значение для z увеличено для улучшения результатов
            max_iterations = 500  # Ограничение на количество итераций для предотвращения бесконечного цикла, уменьшено для повышения производительности
            while z < max_iterations:
                probability = probability_attack_success(z, p_m_prime, p_h_prime)
                print(f"z: {z}, probability: {probability}")  # Отладка для вероятности на каждом шаге
                if probability < 1e-3:  # Изменено на 1e-3 для более точного контроля вероятности успеха
                    blocks_confirmed.append(z)
                    break
                z += 1
            else:
                # Если достигнуто максимальное количество итераций, добавляем предупреждение
                print(f"Warning: Reached max iterations for p_m = {p_m}, D_H = {D_H}, alpha = {alpha}")
                blocks_confirmed.append(z)

        # Построение графиков
        plt.plot(p_m_values, blocks_confirmed, label=f"D_H = {D_H}")

    # Настройки графика
    plt.xlabel("Доля зловмисників")
    plt.ylabel("Кількість блоків підтвердження")
    plt.title(f"Залежність кількості блоків підтвердження від частки зловмисників (alpha = {alpha})")
    plt.legend()
    plt.grid(True)
    plt.show()
