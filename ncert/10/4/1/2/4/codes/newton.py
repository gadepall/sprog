import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
root_finder = ctypes.CDLL('./newton.so')

# Define the C function prototypes
root_finder.newton_raphson.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
root_finder.newton_raphson.restype = ctypes.c_double

root_finder.secant_method.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int]
root_finder.secant_method.restype = ctypes.c_double

# Python function for f(x) for plotting
def f(x):
    return x**2 - 8 * x - 1280

# Compute the roots using Newton-Raphson and Secant methods
tolerance = 1e-6
max_iterations = 100

# Initial guesses for Newton-Raphson and Secant methods
root1_nr = root_finder.newton_raphson(30.0, tolerance, max_iterations)
root2_nr = root_finder.newton_raphson(-50.0, tolerance, max_iterations)

root1_sec = root_finder.secant_method(30.0, 35.0, tolerance, max_iterations)
root2_sec = root_finder.secant_method(-50.0, -45.0, tolerance, max_iterations)

# Print the computed roots
print(f"Newton-Raphson Root 1: {root1_nr}")
print(f"Newton-Raphson Root 2: {root2_nr}")
print(f"Secant Method Root 1: {root1_sec}")
print(f"Secant Method Root 2: {root2_sec}")

# Plot the graph of f(x)
x = np.linspace(-60, 60, 500)
y = f(x)

plt.figure(figsize=(8, 6))
plt.plot(x, y, label='f(x) = x² - 8x - 1280')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)
plt.scatter([root1_nr, root2_nr], [0, 0], color='red', label='Newton-Raphson Roots')
plt.scatter([root1_sec, root2_sec], [0, 0], color='blue', label='Secant Method Roots', marker='x')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid()
plt.show()

