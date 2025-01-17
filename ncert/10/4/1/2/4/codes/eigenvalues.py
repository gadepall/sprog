import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the compiled C shared library
qr_lib = ctypes.CDLL('./eigenvalues.so')

# Define the argument and return types for the C function
qr_lib.find_eigenvalues.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]

# Wrapper function in Python
def find_eigenvalues(a, b, c):
    eigen1 = ctypes.c_double()
    eigen2 = ctypes.c_double()
    qr_lib.find_eigenvalues(a, b, c, ctypes.byref(eigen1), ctypes.byref(eigen2))
    return eigen1.value, eigen2.value

# Coefficients of the quadratic equation
a = 1.0
b = -8.0
c = -1280.0

# Find the roots of the quadratic equation
eigen1, eigen2 = find_eigenvalues(a, b, c)

print("Roots of the quadratic equation are:")
print(f"Root 1: {eigen1}")
print(f"Root 2: {eigen2}")

# Define the quadratic equation
def quadratic(x):
    return a * x**2 + b * x + c

# Generate x values for plotting
x_values = np.linspace(-50, 50, 500)
y_values = quadratic(x_values)

# Plot the quadratic curve
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label=r"$x^2 - 8x - 1280$", color="blue")
plt.axhline(0, color="black", linestyle="--", linewidth=1)

# Highlight the roots
plt.scatter([eigen1, eigen2], [0, 0], color="red", label="Roots", zorder=5)

# Add labels, title, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.show()

