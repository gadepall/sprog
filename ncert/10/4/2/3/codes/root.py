import ctypes
import math
import numpy as np
import matplotlib.pyplot as plt
# Load the shared object file
newton_lib = ctypes.CDLL('./libnewton_raphson.so')

# Define the argument and return types for the Newton-Raphson function
newton_lib.newton_raphson.argtypes = [
    ctypes.c_double,  # a
    ctypes.c_double,  # b
    ctypes.c_double,  # c
    ctypes.c_double,  # initial_guess
    ctypes.c_double,  # tol
    ctypes.c_int,     # max_iter
    ctypes.POINTER(ctypes.c_double)  # root (output parameter)
]
newton_lib.newton_raphson.restype = ctypes.c_int

# Parameters for the quadratic equation: ax^2 + bx + c = 0
a = 1
b = -27
c = 182

tol = 1e-6
max_iter = 100

# Compute the discriminant to determine initial guesses
discriminant = b ** 2 - 4 * a * c

if discriminant < 0:
    print("The equation has complex roots. Newton-Raphson method cannot be used.")
else:
    #initial_guess1 = (-b + math.sqrt(discriminant)) / (2 * a)
    initial_guess1 = 11.0
    #initial_guess2 = (-b - math.sqrt(discriminant)) / (2 * a)
    initial_guess2 = 15.0

    # Allocate memory for roots
    root1 = ctypes.c_double()
    root2 = ctypes.c_double()

    # Find the first root
    if newton_lib.newton_raphson(a, b, c, initial_guess1, tol, max_iter, ctypes.byref(root1)):
        print(f"Root 1: {root1.value:.6f}")
    else:
        print("Failed to find Root 1.")

    # Find the second root
    if newton_lib.newton_raphson(a, b, c, initial_guess2, tol, max_iter, ctypes.byref(root2)):
        print(f"Root 2: {root2.value:.6f}")
    else:
        print("Failed to find Root 2.")

# Define the quadratic equation
def quadratic(x):
    return a * x**2 + b * x + c

# Generate x values for plotting
x_values = np.linspace(10, 20, 500)
y_values = quadratic(x_values)

plt.figure(figsize=(8, 8))
plt.plot(x_values, y_values, label=r"$x^2 - 27x + 182$", color="blue")
plt.axhline(0, color="black", linestyle="--", linewidth=1)

# Highlight the roots
plt.scatter([root1.value, root2.value], [0, 0], color="red", label="Roots", zorder=5)

# Add labels, title, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.legend()

plt.grid()
plt.savefig("../plots/plot1.png")
plt.show()
