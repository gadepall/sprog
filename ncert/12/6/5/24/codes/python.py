import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
solver = ctypes.CDLL("./gradient_solver.so")

# Define argument types for the C functions
solver.find_maximum.argtypes = [
    ctypes.c_double, ctypes.c_int,
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)
]

solver.find_minimum.argtypes = [
    ctypes.c_double, ctypes.c_int,
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)
]

# Function to call the C maximum function
def find_maximum(learning_rate, max_iterations):
    max_x_1 = ctypes.c_double()
    max_x_2 = ctypes.c_double()
    max_val_1 = ctypes.c_double()
    max_val_2 = ctypes.c_double()
    solver.find_maximum(learning_rate, max_iterations, 
                        ctypes.byref(max_x_1), ctypes.byref(max_x_2), 
                        ctypes.byref(max_val_1), ctypes.byref(max_val_2))
    return (max_x_1.value, max_val_1.value), (max_x_2.value, max_val_2.value)

# Function to call the C minimum function
def find_minimum(learning_rate, max_iterations):
    min_x = ctypes.c_double()
    min_val = ctypes.c_double()
    solver.find_minimum(learning_rate, max_iterations, 
                        ctypes.byref(min_x), ctypes.byref(min_val))
    return min_x.value, min_val.value

# Call the C functions
learning_rate = 0.01
max_iterations = 1000

max_1, max_2 = find_maximum(learning_rate, max_iterations)
min_x, min_value = find_minimum(learning_rate, max_iterations)

print(f"Maximum values at: x={max_1[0]} -> {max_1[1]} and x={max_2[0]} -> {max_2[1]}")
print(f"Minimum value at: x={min_x} -> {min_value}")

# Plot the function and extrema
def function(x):
    return (x**2 - x + 1)**(1/3)

x_values = np.linspace(0, 1, 500)
y_values = function(x_values)

plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label="f(x) = (x² - x + 1)^(1/3)")
plt.scatter(max_1[0], max_1[1], color="red", s=100, label="Max Point 1")
plt.scatter(max_2[0], max_2[1], color="orange", s=100, label="Max Point 2")
plt.scatter(min_x, min_value, color="blue", s=100, label="Min Point")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.grid()
plt.savefig('fig.pdf')
plt.show()

