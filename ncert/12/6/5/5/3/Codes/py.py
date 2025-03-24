import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./gradient.so')

# Define argument and return types for the C functions
lib.gradient_descent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.gradient_descent.restype = ctypes.c_double

lib.gradient_ascent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.gradient_ascent.restype = ctypes.c_double

# Parameters for gradient optimization
start_point = 0.0
learning_rate = 0.01
max_iter = 1000
tolerance = 1e-6

# Find minimum and maximum using the C functions
min_x = lib.gradient_descent(start_point, learning_rate, max_iter, tolerance)
max_x = lib.gradient_ascent(start_point, learning_rate, max_iter, tolerance)

def f(x):
    return 4 * x - (x**2) / 2  # Ensure the function returns the correct value

# Evaluate function values for plotting
x_vals = np.linspace(-2, 4.5, 500)
y_vals = 4 * (x_vals) - (x_vals**2) / 2


# Plot the function
plt.figure(figsize=(10, 6))
plt.plot(x_vals, y_vals, label="f(x) = 4x - x^2 / 2", color="blue")

# Plot min and max points
plt.scatter([min_x, max_x], [f(min_x), f(max_x)], color='red', zorder=5,label='Min/Max')
plt.text(min_x, f(min_x), f" Min ", fontsize=12, verticalalignment='bottom')
plt.text(max_x, f(max_x), f" Max ", fontsize=12, verticalalignment='bottom')

plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.grid(True)
plt.show()

