import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library
qr_lib = ctypes.CDLL('./eigenvalues.so')

# Define the argument and return types for the C function
qr_lib.find_eigenvalues.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]

def find_eigenvalues(a, b, c):
    eigen1 = ctypes.c_double()
    eigen2 = ctypes.c_double()
    qr_lib.find_eigenvalues(a, b, c, ctypes.byref(eigen1), ctypes.byref(eigen2))
    return eigen1.value, eigen2.value

# Coefficients of the quadratic equation
a, b, c = 3.0, -2.0 * math.sqrt(6), 2.0
eigen1, eigen2 = find_eigenvalues(a, b, c)

print(f"Roots of the quadratic equation: Root 1: {eigen1:.6f}, Root 2: {eigen2:.6f}")

def quadratic(x):
    return a * x**2 + b * x + c

x_values = np.linspace(-2, 2, 500)
y_values = quadratic(x_values)

plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label=r"$3x^2 - 2\sqrt{6}x + 2$", color="blue")
plt.axhline(0, color="black", linestyle="--", linewidth=1)

# Highlight and annotate the roots
plt.scatter([eigen1, eigen2], [0, 0], color="red", zorder=5)
plt.annotate(f"Root 1: {eigen1:.3f}", (eigen1, 0), textcoords="offset points", xytext=(0, 10), ha='center')
plt.annotate(f"Root 2: {eigen2:.3f}", (eigen2, 0), textcoords="offset points", xytext=(0, -15), ha='center')

plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.show()

