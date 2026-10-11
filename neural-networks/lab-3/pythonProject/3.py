import numpy as np

def tanh(x):
    return np.tanh(x)

def tanh_derivative(x):
    return 1.0 - np.tanh(x) ** 2

np.random.seed(42)
weights_input_hidden1 = np.random.uniform(-1, 1, (3, 2))
weights_hidden1_hidden2 = np.random.uniform(-1, 1, (2, 3))
weights_hidden2_output = np.random.uniform(-1, 1, (3, 1))

input_vector = np.array([[0.5, 0.8, 0.05]])
target_vector = np.array([[0.45]])

hidden1_input = np.dot(input_vector, weights_input_hidden1)
hidden1_output = tanh(hidden1_input)
print("Hidden Layer 1 output:", hidden1_output)

hidden2_input = np.dot(hidden1_output, weights_hidden1_hidden2)
hidden2_output = tanh(hidden2_input)
print("Hidden Layer 2 output:", hidden2_output)

final_input = np.dot(hidden2_output, weights_hidden2_output)
final_output = tanh(final_input)
print("Final output:", final_output)

output_error = target_vector - final_output
delta_output = output_error * tanh_derivative(final_output)
print("Delta output:", delta_output)

hidden2_error = delta_output.dot(weights_hidden2_output.T)
delta_hidden2 = hidden2_error * tanh_derivative(hidden2_output)
print("Delta Hidden Layer 2:", delta_hidden2)

hidden1_error = delta_hidden2.dot(weights_hidden1_hidden2.T)
delta_hidden1 = hidden1_error * tanh_derivative(hidden1_output)
print("Delta Hidden Layer 1:", delta_hidden1)

learning_rate = 0.25
weights_hidden2_output += learning_rate * hidden2_output.T.dot(delta_output)
weights_hidden1_hidden2 += learning_rate * hidden1_output.T.dot(delta_hidden2)
weights_input_hidden1 += learning_rate * input_vector.T.dot(delta_hidden1)

print("Updated weights from Hidden Layer 2 to Output:", weights_hidden2_output)
print("Updated weights from Hidden Layer 1 to Hidden Layer 2:", weights_hidden1_hidden2)
print("Updated weights from Input to Hidden Layer 1:", weights_input_hidden1)
