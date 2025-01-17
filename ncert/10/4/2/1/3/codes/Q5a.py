import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
root_finder = ctypes.CDLL('./root_finder.so')

# Define the C function prototypes
root_finder.newton_raphson.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
root_finder.newton_raphson.restype = ctypes.c_double

root_finder.secant_method.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int]
root_finder.secant_method.restype = ctypes.c_double

# Python function for f(x) for plotting
def f(x):
    return np.sqrt(2) * x**2 + 7 * x + 5 * np.sqrt(2)

# Compute the roots using Newton-Raphson and Secant methods
tolerance = 1e-6
max_iterations = 100

root1_nr = root_finder.newton_raphson(-5, tolerance, max_iterations)
root2_nr = root_finder.newton_raphson(0, tolerance, max_iterations)

root1_sec = root_finder.secant_method(-5, -4, tolerance, max_iterations)
root2_sec = root_finder.secant_method(0, -1, tolerance, max_iterations)

print(f"Newton-Raphson Root 1: {root1_nr}")
print(f"Newton-Raphson Root 2: {root2_nr}")
print(f"Secant Method Root 1: {root1_sec}")
print(f"Secant Method Root 2: {root2_sec}")

# Plot the graph of f(x)
x = np.linspace(-6, 2, 500)
y = f(x)

plt.figure(figsize=(8, 6))
plt.plot(x, y, label='f(x) = √2x² + 7x + 5√2')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)
plt.scatter([root1_nr, root2_nr], [0, 0], color='red', label='Newton-Raphson Roots')
plt.scatter([root1_sec, root2_sec], [0, 0], color='blue', label='Secant Method Roots', marker='x')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid()
plt.show()

