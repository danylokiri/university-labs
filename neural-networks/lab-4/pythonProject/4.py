import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_classification, make_regression
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPClassifier, MLPRegressor
from sklearn.metrics import accuracy_score, classification_report, r2_score, mean_absolute_error, mean_squared_error

n_points = 100 + 2 * 15
n_features_classification = 3
n_classes = 3
n_features_regression = 5
n_targets = 1
learning_rate = 0.25
max_iter = 400
activation_function = 'tanh'
learning_method = 'sgd'

X_classification, y_classification = make_classification(
    n_samples=n_points,
    n_features=n_features_classification,
    n_informative=2,
    n_redundant=0,
    n_classes=n_classes,
    n_clusters_per_class=1,
    random_state=42
)

X_train_cls, X_test_cls, y_train_cls, y_test_cls = train_test_split(
    X_classification, y_classification, test_size=0.2, random_state=42
)

mlp_classifier = MLPClassifier(
    hidden_layer_sizes=(100,),
    activation=activation_function,
    solver=learning_method,
    learning_rate_init=learning_rate,
    max_iter=max_iter,
    random_state=42
)

mlp_classifier.fit(X_train_cls, y_train_cls)

y_pred_cls = mlp_classifier.predict(X_test_cls)
print("Accuracy (classification):", accuracy_score(y_test_cls, y_pred_cls))
print("Classification Report:\n", classification_report(y_test_cls, y_pred_cls))

plt.figure(figsize=(8, 6))
plt.scatter(X_classification[:, 0], X_classification[:, 1], c=y_classification, cmap='viridis', edgecolor='k', s=50)
plt.title("Classification Data")
plt.xlabel("Feature 1")
plt.ylabel("Feature 2")
plt.show()

X_regression, y_regression = make_regression(
    n_samples=n_points,
    n_features=n_features_regression,
    n_targets=n_targets,
    noise=0.1,
    random_state=42
)

X_train_reg, X_test_reg, y_train_reg, y_test_reg = train_test_split(
    X_regression, y_regression, test_size=0.2, random_state=42
)

mlp_regressor = MLPRegressor(
    hidden_layer_sizes=(100,),
    activation=activation_function,
    solver=learning_method,
    learning_rate_init=learning_rate,
    max_iter=max_iter,
    random_state=42
)

mlp_regressor.fit(X_train_reg, y_train_reg)

y_pred_reg = mlp_regressor.predict(X_test_reg)
print("R2 Score (regression):", r2_score(y_test_reg, y_pred_reg))
print("Mean Absolute Error (MAE):", mean_absolute_error(y_test_reg, y_pred_reg))
print("Mean Squared Error (MSE):", mean_squared_error(y_test_reg, y_pred_reg))

plt.figure(figsize=(8, 6))
plt.scatter(y_test_reg, y_pred_reg, edgecolor='k', s=50)
plt.plot([y_test_reg.min(), y_test_reg.max()], [y_test_reg.min(), y_test_reg.max()], 'r--', lw=2)
plt.title("Regression Results")
plt.xlabel("True Values")
plt.ylabel("Predicted Values")
plt.show()
