import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./c.so')

# Define the return type for the C functions
gradient_lib.find_critical_point.restype = ctypes.c_double

# Define a function to find the critical points
def find_critical_point(start, learning_rate, max_iterations, tolerance, ascent):
    gradient_lib.find_critical_point.argtypes = [
        ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double, ctypes.c_int
    ]
    return gradient_lib.find_critical_point(start, learning_rate, max_iterations, tolerance, ascent)

# Parameters for the critical point calculation
learning_rate = 0.1
max_iterations = 1000
tolerance = 1e-6

# Find the local minimum
x_min_start = -1.0  # Starting point near the expected minimum
x_min = find_critical_point(x_min_start, learning_rate, max_iterations, tolerance, 0)

# Find the local maximum
x_max_start = 1.0  # Starting point near the expected maximum
x_max = find_critical_point(x_max_start, learning_rate, max_iterations, tolerance, 1)

# Define the function g(x) = x^3 - 3x
def function_g(x):
    return x**3 - 3*x

# Evaluate the function at the critical points
g_min = function_g(x_min)
g_max = function_g(x_max)

# Print results
print(f"Local Minimum:\nx = {x_min}, g(x) = {g_min}")
print(f"Local Maximum:\nx = {x_max}, g(x) = {g_max}")

# Generate x values for plotting
x_values = np.linspace(-2, 2, 500)
y_values = function_g(x_values)

# Plot the function
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label=r"$g(x) = x^3 - 3x$", color='blue')
plt.scatter([x_min], [g_min], color='red', label=f'Local Minimum: ({x_min:.2f}, {g_min:.2f})')
plt.scatter([x_max], [g_max], color='green', label=f'Local Maximum: ({x_max:.2f}, {g_max:.2f})')
plt.xlabel('x')
plt.ylabel('g(x)')
plt.axhline(0, color='black', linewidth=0.8, linestyle='--')
plt.axvline(0, color='black', linewidth=0.8, linestyle='--')
plt.legend()
plt.grid()
plt.show()

