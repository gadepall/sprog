import ctypes
import math
import matplotlib.pyplot as plt
import numpy as np

# Load the shared library
# Replace the path with the correct location of the shared library
c_library = ctypes.CDLL('./code.so')

# Define the structure to hold the roots
class Roots(ctypes.Structure):
    _fields_ = [("root1", ctypes.c_double), ("root2", ctypes.c_double)]

# Set the argument and return types for the C function
c_library.eigen_values.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]
c_library.eigen_values.restype = Roots

def f(x):
    return 4*x**2+4*np.sqrt(3)*x+3
# Python function to call the C function
def find_roots(a, b, c):
    roots = c_library.eigen_values(a, b, c)
    return roots.root1, roots.root2

# Example usage for the equation 4x^2 + 4√3x + 3 = 0
a = 4
b = 4 * math.sqrt(3)
c = 3

root1, root2 = find_roots(a, b, c)
print(f"Root 1: {root1:.3f}, Root 2: {root2:.3f}")

# Plot the graph of f(x)
x = np.linspace(-6, 2, 500)
y = f(x)
plt.figure(figsize=(8, 6))
plt.plot(x, y, label='f(x) = 4x² + 4√3x + 3')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)
plt.scatter([root1],[0], color='green', label='root by eigen value')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid()
plt.show()