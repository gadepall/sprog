import ctypes
import math
import matplotlib.pyplot as plt
import numpy as np

# Load the shared library
# Replace the path with the correct location of the shared library
c_library = ctypes.CDLL('./output.so')

# Define the structure to hold the roots
class Roots(ctypes.Structure):
    _fields_ = [("root1", ctypes.c_double), ("root2", ctypes.c_double)]

# Set the argument and return types for the C function
c_library.eigen_values.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]
c_library.eigen_values.restype = Roots

def f(x):
    return 2*x**2 + 4*x - 286

# Python function to call the C function
def find_roots(a, b, c):
    roots = c_library.eigen_values(a, b, c)
    return roots.root1, roots.root2

# Example usage for the equation 2x^2 + 4x - 286 = 0
a = 2
b = 4
c = -286

root1, root2 = find_roots(a, b, c)
print(f"Root 1: {root1:.3f}, Root 2: {root2:.3f}")

# Plot the graph of f(x)
x = np.linspace(-20, 20, 500)
y = f(x)
plt.figure(figsize=(8, 6))
plt.plot(x, y, label='f(x) = 2x² + 4x − 286')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)
# Plot and label the roots
plt.scatter([root1, root2], [0, 0], color='green', label='Roots')
plt.text(root1, 0.5, f'Root 1 ({root1:.2f})', color='green', fontsize=10, ha='center')
plt.text(root2, 0.5, f'Root 2 ({root2:.2f})', color='green', fontsize=10, ha='center')

# Labels and legend
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid()
plt.title('Quadratic Equation: 2x² + 4x − 286 = 0')
plt.show()

