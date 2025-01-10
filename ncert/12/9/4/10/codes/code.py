import math
import ctypes
import matplotlib.pyplot as plt
import numpy as np

# Load the .so file
lib = ctypes.CDLL('./code.so')

# Define argument and return types for C functions
lib.vals.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_int]
lib.vals.restype = None

lib.dydx.argtypes = [ctypes.c_float, ctypes.c_float]
lib.dydx.restype = ctypes.c_float

# Set initial values and parameters
x = ctypes.c_float(-0.693)
y = ctypes.c_float(1.107)
n = 5  # Number of iterations
h = 0.01  # Step size (not used here, but may be relevant for your C implementation)

# Create arrays to store results for plotting
x_vals = []
y_vals = []

# Generate values using the C function
for _ in range(n):
    x_vals.append(float(x.value))
    y_vals.append(float(y.value))
    # Call the C function to update x and y
    lib.vals(ctypes.byref(x), ctypes.byref(y), 1)

# Additional plot: Theoretical values based on a custom function
x_theory = np.linspace(-4, 4, 20)
y_theory = np.arctan(1 / ((math.e) ** x_theory - 1)) + 2.2

# Plot the theoretical function
plt.plot(x_theory, y_theory, label="Theory", color='chartreuse', linestyle='-')

# Plot the simulation results with dotted style
plt.plot(x_vals, y_vals, label="Simulation", color='midnightblue', linestyle=':')

# Customize plot
plt.xlabel("x-axis", fontsize=14)
plt.ylabel("y-axis", fontsize=14)
plt.title("Simulation vs. Theoretical Values", fontsize=16)

# Customize axis spines for thick black axes
ax = plt.gca()
ax.spines['bottom'].set_color('black')
ax.spines['bottom'].set_linewidth(2)
ax.spines['left'].set_color('black')
ax.spines['left'].set_linewidth(2)

# Customize tick parameters for thicker black ticks
ax.tick_params(axis='both', colors='black', width=2, length=6)

# Add grid and legend
plt.grid(True)
plt.legend(fontsize=12)

# Show plot
plt.tight_layout()
plt.show()

