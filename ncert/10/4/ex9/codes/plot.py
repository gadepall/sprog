import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import ctypes

# Load the shared library
newton_lib = ctypes.CDLL("./newton_roots.so")

# Define the complex structure for C interface
class Complex(ctypes.Structure):
    _fields_ = [("real", ctypes.c_double), ("imag", ctypes.c_double)]

# Define return type and arguments for Newton's method
newton_lib.newton_method.restype = Complex
newton_lib.newton_method.argtypes = [Complex, ctypes.c_double, ctypes.c_int]

# Define a wrapper to use Newton's method for complex numbers
def newton_root_complex(initial_guess, tolerance, max_iter):
    guess = Complex(initial_guess.real, initial_guess.imag)
    result = newton_lib.newton_method(guess, tolerance, max_iter)
    return result.real + 1j * result.imag

# Define the polynomial
def f_complex(z):
    """Evaluate the polynomial f(z) = 4z^2 + 3z + 5."""
    return 4 * z**2 + 3 * z + 5

# Create a grid for complex numbers
x_real = np.linspace(-5, 5, 200)  # Real part of x
x_imag = np.linspace(-5, 5, 200)  # Imaginary part of x
real, imag = np.meshgrid(x_real, x_imag)
z_grid = real + 1j * imag

# Evaluate the polynomial on the grid
f_values = f_complex(z_grid)
f_real = f_values.real
f_imag = f_values.imag

# Find a root using Newton's method
initial_guess = -2 + 2j
tolerance = 1e-6
max_iter = 100
newton_root = newton_root_complex(initial_guess, tolerance, max_iter)

# Prepare for 3D plotting
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')

# Plot the surface of the real part of f(x)
surf = ax.plot_surface(
    real, f_real, imag, cmap="viridis", alpha=0.8, edgecolor='none'
)

# Mark the root found using Newton's method
ax.scatter(
    newton_root.real, f_complex(newton_root).real, newton_root.imag,
    color="red", label="Newton's Method Root", s=100
)

# Mark the actual roots
actual_roots = [(-3 + 1j * np.sqrt(71)) / 8, (-3 - 1j * np.sqrt(71)) / 8]
for root in actual_roots:
    ax.scatter(
        root.real, f_complex(root).real, root.imag,
        color="blue", label="Actual Root", s=100
    )

# Add labels, title, and legend
ax.set_xlabel("Real Part of x")
ax.set_ylabel("f(x) (Real Part)")
ax.set_zlabel("Imaginary Part of x")
ax.set_title("3D Plot of f(x) with Newton's Method and Roots")
ax.legend()

# Show the plot
plt.show()

