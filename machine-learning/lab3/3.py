import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import LabelEncoder
from sklearn.tree import DecisionTreeClassifier, plot_tree
from sklearn.metrics import classification_report, confusion_matrix

df = pd.read_csv(r"D:\Uknord5he\BH3\ml_course\3\csgo_round_snapshots.csv")
df = df.dropna()
df.head()


# 2 3
df.info()
df.describe()

plt.figure(figsize=(6, 4))
sns.countplot(data=df, x='round_winner')
plt.title("Class Distribution")
plt.xlabel("Round Winner")
plt.ylabel("Count")
plt.tight_layout()
plt.show()

plt.figure(figsize=(10, 8))
sns.heatmap(df.corr(numeric_only=True), cmap='coolwarm', center=0)
plt.title("Correlation Matrix")
plt.tight_layout()
plt.show()

features_to_plot = ['ct_health', 't_health', 'ct_money', 't_money']
df_sample = df[features_to_plot + ['round_winner']].sample(1000)

sns.pairplot(df_sample, hue='round_winner', diag_kind='hist')
plt.suptitle("Feature Distribution by Class", y=1.02)
plt.show()

# 4
class_mapping = {'CT': 0, 'T': 1}
rev_class_mapping = {v: k for k, v in class_mapping.items()}

X = df.drop(columns=['map', 'round_winner'])
y = df['round_winner'].map(class_mapping)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)
#5
from sklearn.ensemble import RandomForestClassifier

rf_clf = RandomForestClassifier(random_state=42)
rf_clf.fit(X_train, y_train)
y_pred_rf = rf_clf.predict(X_test)

print(classification_report(y_test, y_pred_rf, target_names=rev_class_mapping.values()))

conf_matrix_rf = confusion_matrix(y_test, y_pred_rf)
plt.figure(figsize=(6, 4))
sns.heatmap(conf_matrix_rf, annot=True, fmt='d', xticklabels=rev_class_mapping.values(), yticklabels=rev_class_mapping.values(), cmap='Blues')
plt.xlabel("Predicted")
plt.ylabel("True")
plt.title("Random Forest Confusion Matrix")
plt.tight_layout()
plt.show()
#6
from sklearn.model_selection import GridSearchCV

param_grid = {
    'n_estimators': [50, 100],
    'max_depth': [None, 5, 10],
    'min_samples_split': [2, 5]
}

grid_rf = GridSearchCV(RandomForestClassifier(random_state=42), param_grid, cv=5, scoring='accuracy')
grid_rf.fit(X_train, y_train)

best_rf = grid_rf.best_estimator_
y_pred_best_rf = best_rf.predict(X_test)

print("Best Parameters:", grid_rf.best_params_)
print(classification_report(y_test, y_pred_best_rf, target_names=rev_class_mapping.values()))

conf_matrix_best_rf = confusion_matrix(y_test, y_pred_best_rf)
plt.figure(figsize=(6, 4))
sns.heatmap(conf_matrix_best_rf, annot=True, fmt='d', xticklabels=rev_class_mapping.values(), yticklabels=rev_class_mapping.values(), cmap='Greens')
plt.xlabel("Predicted")
plt.ylabel("True")
plt.title("Optimized Random Forest Confusion Matrix")
plt.tight_layout()
plt.show()
#7

print("Default Random Forest Accuracy:", rf_clf.score(X_test, y_test))
print("Optimized Random Forest Accuracy:", best_rf.score(X_test, y_test))

print("Best Parameters:", grid_rf.best_params_)

if best_rf.score(X_test, y_test) > rf_clf.score(X_test, y_test):
    print("Optimization improved the model.")
else:
    print("Optimization did not improve the model.")
