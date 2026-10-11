import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs
from sklearn.model_selection import train_test_split

X, y = make_blobs(n_samples=100, centers=2, random_state=42)
y = np.where(y == 0, -1, 1)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.15, random_state=42)

weights = np.random.uniform(-0.5, 0.5, X.shape[1])
bias = np.random.uniform(-0.5, 0.5)
learning_rate = 0.01
epochs = 200

#тренування
def train_perceptron(X, y, weights, bias, learning_rate, epochs):
    for epoch in range(epochs):
        for xi, target in zip(X, y):
            output = np.dot(xi, weights) + bias
            error = target - output
            weights += learning_rate * error * xi
            bias += learning_rate * error
    return weights, bias

weights, bias = train_perceptron(X_train, y_train, weights, bias, learning_rate, epochs)

def predict(X, weights, bias):
    return np.where(np.dot(X, weights) + bias >= 0.0, 1, -1)

y_pred = predict(X_test, weights, bias)
accuracy = np.mean(y_pred == y_test)
print(f"Точність на тестовій вибірці: {accuracy * 100:.2f}%")

#plt.scatter(X_test[:, 0], X_test[:, 1], c=y_pred, cmap='bwr', marker='s')
#plt.title("Тестова вибірка з прогнозованими значеннями")
#plt.show()