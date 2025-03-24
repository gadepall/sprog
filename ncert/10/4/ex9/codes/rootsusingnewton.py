import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
newton_lib = ctypes.CDLL('./newton_complex.so')

# Configure the function signature for find_roots
newton_lib.find_roots.restype = None
newton_lib.find_roots.argtypes = [
    ctypes.c_double,      # Tolerance
    ctypes.c_int,         # Max iterations
    ctypes.POINTER(ctypes.c_double),  # Real parts array
    ctypes.POINTER(ctypes.c_double)   # Imaginary parts array
]

# Prepare arrays for roots
real_parts = (ctypes.c_double * 2)()
imag_parts = (ctypes.c_double * 2)()

# Call the C function
tolerance = 1e-6
max_iterations = 1000
newton_lib.find_roots(tolerance, max_iterations, real_parts, imag_parts)

# Extract roots from Newton's method
newton_roots = [(real_parts[i], imag_parts[i]) for i in range(2)]

# Roots obtained using Completing the Square Method
# These are pre-calculated for the polynomial: f(x) = 4x^2 + 3x + 5
completing_square_roots = [
    (-3 / 8, np.sqrt(71) / 8),   # First root
    (-3 / 8, -np.sqrt(71) / 8)  # Second root
]

# Define the polynomial for plotting
def f(x):
    return 4 * x**2 + 3 * x + 5

# Generate grid for real and imaginary parts
x_real = np.linspace(-2, 2, 500)
x_imag = np.linspace(-2, 2, 500)
X_real, X_imag = np.meshgrid(x_real, x_imag)
F = f(X_real + 1j * X_imag)

# Plot the function
plt.figure(figsize=(10, 8))
plt.contour(X_real, X_imag, np.abs(F), levels=30, cmap='viridis')
plt.colorbar(label='|f(x)|')
plt.title("Complex Roots of f(x) = 4x^2 + 3x + 5")
plt.xlabel("Real Part of x")
plt.ylabel("Imaginary Part of x")

# Mark roots from Newton's method
for re, im in newton_roots:
    plt.scatter(re, im, color="red", label=f"Newton Root ({re:.2f}, {im:.2f}i)")

# Mark roots from Completing the Square method
for re, im in completing_square_roots:
    plt.scatter(re, im, color="blue", label=f"Completing Square Root ({re:.2f}, {im:.2f}i)")

# Avoid duplicate labels
handles, labels = plt.gca().get_legend_handles_labels()
by_label = dict(zip(labels, handles))
plt.legend(by_label.values(), by_label.keys())

plt.grid(True)
plt.show()

