import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the compiled C library
lib = ctypes.CDLL('./gradient_extrema.so')

# Define function prototypes
lib.gradient_descent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.gradient_descent.restype = ctypes.c_double

lib.gradient_ascent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.gradient_ascent.restype = ctypes.c_double

# Define the Python function
def f(x):
    return x + np.sin(2 * x)

# Gradient descent/ascent parameters
learning_rate = 0.01
max_iter = 10000
tol = 1e-6
start_points = np.linspace(0, 2 * np.pi, 50)  # Multiple starting points

# calling c code
critical_points = []
for x0 in start_points:
    min_x = lib.gradient_descent(x0, learning_rate, max_iter, tol)
    max_x = lib.gradient_ascent(x0, learning_rate, max_iter, tol)
    if not any(abs(min_x - cp) < 1e-3 for cp in critical_points):
        critical_points.append(min_x)
    if not any(abs(max_x - cp) < 1e-3 for cp in critical_points):
        critical_points.append(max_x)

critical_points = np.unique(np.array(critical_points))

# Evaluate the function at the critical points
extrema = [(x, f(x)) for x in critical_points]

# Separate minima and maxima
minima = []
maxima = []
for x, y in extrema:
    if np.sign(f(x + 1e-3) - f(x - 1e-3)) > 0:  # Check slope for minima
        minima.append((x, y))
    else:
        maxima.append((x, y))

# Plot the function and both minima and maxima
x_vals = np.linspace(0, 2 * np.pi, 500)
y_vals = f(x_vals)

plt.figure(figsize=(10, 6))
plt.plot(x_vals, y_vals, label='f(x) = x + sin(2x)', color='blue')

# minima points
for x, y in minima:
    if 0 <= x <= 2 * np.pi:  # Plot only minima within the range [0, 2pi]
        plt.scatter(x, y, color='green', zorder=5, label=f"Min: ({x:.2f}, {y:.2f})")

# maxima points
for x, y in maxima:
    if 0 <= x <= 2 * np.pi:  # Plot only maxima within the range [0, 2pi]
        plt.scatter(x, y, color='red', zorder=5, label=f"Max: ({x:.2f}, {y:.2f})")

# Finalize the plot

plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend(loc='best')
plt.grid(True)
plt.show()

# Display minima and maxima
print("Minima:", minima)
print("Maxima:", maxima)

