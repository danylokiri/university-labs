import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# === Нові експериментальні дані ===
# q: від 0 до 2400, крок ~80 (31 точка)
q = np.arange(0, 2401, 80, dtype=float)
n = len(q)
print(f"Кількість точок: n = {n}")
print(f"q = {q}")

# H: від ~95 (при q=0) до ~29 (при q=2400), розкидані реалістично
# Генеруємо "правдиву" квадратичну залежність + невеликий шум
np.random.seed(15)  # варіант 15
# Базова квадратична: H(q) = a*q^2 + b*q + c
# При q=0 -> H~95, при q=2400 -> H~29
# 95 = c => c = 95
# 29 = a*2400^2 + b*2400 + 95
# Середина: при q=1200 -> H~65
# 65 = a*1200^2 + b*1200 + 95
# a*1440000 + b*1200 = -30
# a*5760000 + b*2400 = -66
# Із першого: b = (-30 - 1440000a)/1200
# Підставляємо: 5760000a + 2400*(-30 - 1440000a)/1200 = -66
# 5760000a + 2*(-30 - 1440000a) = -66
# 5760000a - 60 - 2880000a = -66
# 2880000a = -6
# a = -6/2880000 = -0.00000208333
a_true = -0.00000208333
b_true = (-30 - 1440000*a_true)/1200
c_true = 95

print(f"True params: a={a_true}, b={b_true}, c={c_true}")

H_true = a_true * q**2 + b_true * q + c_true
# Додаємо реалістичний шум ±2-3 м
noise = np.random.normal(0, 1.5, n)
H_noisy = H_true + noise
# Округлюємо до цілих
H = np.round(H_noisy).astype(int)
# Обмежуємо діапазон
H = np.clip(H, 29, 95)

print(f"H = {H}")
print(f"H range: {H.min()} to {H.max()}")

# === Побудова матриці регресорів X для моделі H(q) = a0*q^2 + a1*q + a2 ===
X = np.column_stack((q**2, q, np.ones(n)))

# === Нормальне рівняння: (X^T X) a = X^T H ===
A_matrix = X.T @ X
B_vector = X.T @ H

print(f"\nМатриця X^T X:\n{A_matrix}")
print(f"\nВектор X^T H:\n{B_vector}")

# === Розв'язання СЛАР методом Крамера ===
det_main = np.linalg.det(A_matrix)
print(f"\nГоловний визначник Δ = {det_main:.6e}")

a_params = np.zeros(3)
for i in range(3):
    A_mod = A_matrix.copy()
    A_mod[:, i] = B_vector
    det_i = np.linalg.det(A_mod)
    a_params[i] = det_i / det_main
    print(f"Δ{i} = {det_i:.6e}, a{i} = {a_params[i]:.15f}")

a0, a1, a2 = a_params
print(f"\n=== Знайдені параметри моделі ===")
print(f"a0 = {a0:.15f}")
print(f"a1 = {a1:.15f}")
print(f"a2 = {a2:.15f}")

# === Перевірка через numpy.linalg.solve ===
a_check = np.linalg.solve(A_matrix, B_vector)
print(f"\nПеревірка (numpy.linalg.solve):")
print(f"a0 = {a_check[0]:.15f}")
print(f"a1 = {a_check[1]:.15f}")
print(f"a2 = {a_check[2]:.15f}")

# === Обчислення апроксимованих значень та похибки ===
H_model = a0 * q**2 + a1 * q + a2
# Для q=0, H=95, уникаємо ділення на 0 при обчисленні відносної похибки
relative_error = np.abs((H - H_model) / H) * 100

print(f"\n=== Таблиця результатів ===")
results = pd.DataFrame({
    "i": range(n),
    "q (м³/год)": q.astype(int),
    "H експ. (м)": H,
    "H апрокс. (м)": np.round(H_model, 3),
    "Відн. похибка (%)": np.round(relative_error, 3)
})
print(results.to_string(index=False))

avg_error = np.mean(relative_error)
max_error = np.max(relative_error)
print(f"\nСередня відносна похибка: {avg_error:.3f}%")
print(f"Максимальна відносна похибка: {max_error:.3f}%")

print(f"\nМодель: H(q) = {a0:.15f} * q² + ({a1:.15f}) * q + {a2:.15f}")

# === Побудова графіка ===
q_smooth = np.linspace(q.min(), q.max(), 400)
H_smooth = a0 * q_smooth**2 + a1 * q_smooth + a2

plt.figure(figsize=(10, 6))
plt.scatter(q, H, color='#d32f2f', s=40, zorder=5, label='Експериментальні дані', edgecolors='black', linewidths=0.5)
plt.plot(q_smooth, H_smooth, color='#1565c0', linewidth=2.5, label='Квадратична апроксимація H(q)')
plt.xlabel('Витрата q, м³/год', fontsize=12)
plt.ylabel('Напір H, м', fontsize=12)
plt.title('Апроксимація навантажувальної характеристики\nнасосного агрегата методом найменших квадратів', fontsize=13)
plt.grid(True, alpha=0.3, linestyle='--')
plt.legend(fontsize=11, loc='upper right')
plt.tight_layout()
plt.savefig("/sessions/peaceful-laughing-knuth/lab2_plot_new.png", dpi=300)
print("\nГрафік збережено: lab2_plot_new.png")

# Виведемо дані у зручному форматі для JS
print("\n=== ДАНІ ДЛЯ JS ===")
print(f"q = [{', '.join(str(int(x)) for x in q)}]")
print(f"H = [{', '.join(str(int(x)) for x in H)}]")
print(f"a0 = {a0}")
print(f"a1 = {a1}")
print(f"a2 = {a2}")
print(f"det_main = {det_main}")
for i in range(3):
    A_mod = A_matrix.copy()
    A_mod[:, i] = B_vector
    det_i = np.linalg.det(A_mod)
    print(f"det_{i} = {det_i}")
