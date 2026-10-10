import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix, classification_report
import seaborn as sns
from tensorflow import keras
from keras import layers

df = datasets.fetch_openml(data_id=1480, as_frame=True).frame

df['Class'] = LabelEncoder().fit_transform(df['Class'])

df_target = df['Class']
df_data = df.drop(['Class'], axis=1)

X_train, X_test, Y_train, Y_test = train_test_split(df_data, df_target, test_size=0.2, random_state=42)

scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

num_classes = len(df['Class'].unique())
Y_train = keras.utils.to_categorical(Y_train, num_classes=num_classes)
Y_test = keras.utils.to_categorical(Y_test, num_classes=num_classes)

model = keras.Sequential(
    [
        layers.Dense(64, activation='relu', input_shape=(X_train.shape[1],)),
        layers.Dense(32, activation='relu'),
        layers.Dense(num_classes, activation='softmax')
    ]
)

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=["accuracy"])

history = model.fit(X_train, Y_train, epochs=50, validation_split=0.2)

test_loss, test_accuracy = model.evaluate(X_test, Y_test)
print("Test Loss:", test_loss)
print("Accuracy:", test_accuracy)

y_predict = model.predict(X_test)

y_test_class = np.argmax(Y_test, axis=1)
y_pred_class = np.argmax(y_predict, axis=1)

conf_mtx = confusion_matrix(y_test_class, y_pred_class)

plt.figure(figsize=(7, 5))
sns.heatmap(conf_mtx, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predicted Label')
plt.ylabel('True Label')
plt.title('Confusion Matrix')
plt.show()

print(classification_report(y_test_class, y_pred_class))

plt.figure(figsize=(12, 4))

plt.subplot(1, 2, 1)
plt.plot(history.history['loss'], label='Training Loss')
plt.plot(history.history['val_loss'], label='Validation Loss')
plt.title('Training and Validation Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss Value')
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(history.history['accuracy'], label='Training Accuracy')
plt.plot(history.history['val_accuracy'], label='Validation Accuracy')
plt.title('Training and Validation Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy Value')
plt.legend()

plt.tight_layout()
plt.show()

model_exp_a = keras.Sequential(
    [
        layers.Dense(128, activation='relu', input_shape=(X_train.shape[1],)),
        layers.Dense(64, activation='relu'),
        layers.Dense(32, activation='relu'),
        layers.Dense(num_classes, activation='softmax')
    ]
)

model_exp_a.compile(optimizer='adam', loss='categorical_crossentropy', metrics=["accuracy"])
history_exp_a = model_exp_a.fit(X_train, Y_train, epochs=50, validation_split=0.2)

test_loss_a, test_accuracy_a = model_exp_a.evaluate(X_test, Y_test)
print("Experiment A - Test Loss:", test_loss_a)
print("Experiment A - Accuracy:", test_accuracy_a)

model_exp_b = keras.Sequential(
    [
        layers.Dense(64, activation='tanh', input_shape=(X_train.shape[1],)),
        layers.Dense(32, activation='tanh'),
        layers.Dense(num_classes, activation='softmax')
    ]
)

model_exp_b.compile(optimizer='adam', loss='categorical_crossentropy', metrics=["accuracy"])
history_exp_b = model_exp_b.fit(X_train, Y_train, epochs=50, validation_split=0.2)

test_loss_b, test_accuracy_b = model_exp_b.evaluate(X_test, Y_test)
print("Experiment B - Test Loss:", test_loss_b)
print("Experiment B - Accuracy:", test_accuracy_b)

model_exp_c = keras.Sequential(
    [
        layers.Dense(64, activation='relu', input_shape=(X_train.shape[1],)),
        layers.Dense(32, activation='relu'),
        layers.Dense(num_classes, activation='softmax')
    ]
)

model_exp_c.compile(optimizer='sgd', loss='categorical_crossentropy', metrics=["accuracy"])
history_exp_c = model_exp_c.fit(X_train, Y_train, epochs=50, validation_split=0.2)

test_loss_c, test_accuracy_c = model_exp_c.evaluate(X_test, Y_test)
print("Experiment C - Test Loss:", test_loss_c)
print("Experiment C - Accuracy:", test_accuracy_c)

plt.figure(figsize=(10, 5))
plt.plot(history.history['loss'], label='Baseline Loss')
plt.plot(history_exp_a.history['loss'], label='Experiment A Loss')
plt.plot(history_exp_b.history['loss'], label='Experiment B Loss')
plt.plot(history_exp_c.history['loss'], label='Experiment C Loss')
plt.title('Loss Comparison')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.legend()
plt.show()
