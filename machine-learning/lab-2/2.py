import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import warnings

warnings.filterwarnings('ignore')

from sklearn.model_selection import train_test_split, KFold, cross_val_score, GridSearchCV
from sklearn.preprocessing import StandardScaler, MinMaxScaler, LabelEncoder, OneHotEncoder
from sklearn.compose import ColumnTransformer
from sklearn.pipeline import Pipeline

from sklearn.linear_model import LinearRegression, Lasso, Ridge, BayesianRidge
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor, GradientBoostingRegressor
from sklearn.svm import SVR
from xgboost import XGBRegressor
from sklearn.neighbors import KNeighborsRegressor

from sklearn import metrics

#1
df = pd.read_csv("CrabAgePrediction.csv")
print(df.info())
print(df.describe())
print(df.isnull().sum())
print("Duplicates:", df.duplicated().sum())

cat_cols = df.select_dtypes(include='object').columns
for col in cat_cols:
    print(f"Unique values in '{col}': {df[col].unique()}")

plt.figure(figsize=(6, 4))
sns.histplot(df["Age"], bins=20, kde=True)
plt.title("Target Distribution: Age")
plt.tight_layout()
plt.show()

#2
df = pd.get_dummies(df, columns=["Sex"], drop_first=True)
print(df.dtypes)

#3
for col in df.drop("Age", axis=1).columns:
    plt.figure(figsize=(6, 4))
    sns.histplot(df[col], kde=True, bins=30)
    plt.title(f"Distribution of {col}")
    plt.tight_layout()
    plt.show()

plt.figure(figsize=(10, 6))
sns.heatmap(df.corr(), annot=True, cmap="coolwarm", fmt=".2f")
plt.title("Correlation Heatmap")
plt.tight_layout()
plt.show()

#4
X = df.drop("Age", axis=1)
y = df["Age"]
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

#5
models = {
    "Linear Regression": LinearRegression(),
    "Decision Tree": DecisionTreeRegressor(random_state=42),
    "Random Forest": RandomForestRegressor(random_state=42),
    "XGBoost": XGBRegressor(random_state=42, verbosity=0),
    "SVR": SVR(),
    "Bayesian Ridge": BayesianRidge()
}

kf = KFold(n_splits=5, shuffle=True, random_state=42)

def evaluate_model(name, model):
    mae = -cross_val_score(model, X_train_scaled, y_train, cv=kf, scoring="neg_mean_absolute_error").mean()
    rmse = np.sqrt(-cross_val_score(model, X_train_scaled, y_train, cv=kf, scoring="neg_mean_squared_error").mean())
    r2 = cross_val_score(model, X_train_scaled, y_train, cv=kf, scoring="r2").mean()
    return [name, round(mae, 3), round(rmse, 3), round(r2, 3)]

cv_results = [evaluate_model(name, model) for name, model in models.items()]
cv_results_df = pd.DataFrame(cv_results, columns=["Model", "CV MAE", "CV RMSE", "CV R2"])
print(cv_results_df.sort_values("CV R2", ascending=False))

#dodatkovo
test_results = []

for name, model in models.items():
    model.fit(X_train_scaled, y_train)
    y_pred = model.predict(X_test_scaled)
    mae = metrics.mean_absolute_error(y_test, y_pred)
    rmse = np.sqrt(metrics.mean_squared_error(y_test, y_pred))
    r2 = metrics.r2_score(y_test, y_pred)
    test_results.append([name, round(mae, 3), round(rmse, 3), round(r2, 3)])

test_results_df = pd.DataFrame(test_results, columns=["Model", "Test MAE", "Test RMSE", "Test R2"])
print(test_results_df.sort_values("Test R2", ascending=False))

plt.figure(figsize=(10, 5))
sns.barplot(x="Model", y="Test R2", data=test_results_df.sort_values("Test R2", ascending=False))
plt.title("Test R² Score by Model")
plt.ylabel("R² Score")
plt.xlabel("Model")
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()

best_model = RandomForestRegressor(random_state=42)
best_model.fit(X_train_scaled, y_train)
y_pred_final = best_model.predict(X_test_scaled)

plt.figure(figsize=(6, 6))
sns.scatterplot(x=y_test, y=y_pred_final)
plt.xlabel("True Age")
plt.ylabel("Predicted Age")
plt.title("True vs Predicted Age")
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--')
plt.tight_layout()
plt.show()

plt.figure(figsize=(6, 4))
sns.histplot(y_test, color='blue', label='True', kde=True)
sns.histplot(y_pred_final, color='orange', label='Predicted', kde=True)
plt.title("Distribution: True vs Predicted")
plt.legend()
plt.tight_layout()
plt.show()
