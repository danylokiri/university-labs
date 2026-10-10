import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Параметри моделі (варіант 15)
q0 = 100              # базовий рівень споживання
alpha = 0.6            # коефіцієнт зниження до моменту t1
t1 = 6                 # момент зміни математичного очікування (год)
T = 24                 # період спостереження (год)
A = 0.5 * q0           # амплітуда синусоїди
omega = np.pi / 6      # циклічна частота
phi = np.pi            # фазовий зсув
disp = 0.24            # індивідуальне значення дисперсії (варіант 15)

# Масив часу
time = np.linspace(0, T, 500)

# Математичне очікування Mq (формула 2)
Mq = np.where(time < t1, alpha * q0, q0)

# Детермінована складова (формула 1)
q_det = Mq + A * np.sin(omega * time + phi)

# Стохастична модель із випадковим відхиленням
np.random.seed(15)
noise = np.random.normal(loc=0.0, scale=disp * Mq, size=time.shape)
q_stoch = q_det + noise

# Побудова графіка
plt.figure(figsize=(12, 6))

plt.step(time, Mq, where='post', linestyle='--', linewidth=2,
         color='purple', label=r'Математичне очікування $Mq_j^{(H)}$')

plt.plot(time, q_det, color='darkorange', linewidth=2,
         label='Детермінована складова')

plt.plot(time, q_stoch, color='steelblue', alpha=0.75, linewidth=1.8,
         label=f'Стохастична модель з дисперсією {disp}')

plt.title('Імітаційна модель технологічного процесу', fontsize=14)
plt.xlabel('Час (години)', fontsize=12)
plt.ylabel('Споживання продукту', fontsize=12)
plt.xticks(np.arange(0, 25, 1))
plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig("simulation_model.png", dpi=300)
print("Графік збережено: simulation_model.png")
