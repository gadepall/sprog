# Import necessary libraries
import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_double

# Load the shared library
lib = CDLL("./func.so")

# Specify the argument and return types for the C function
lib.integrated.argtypes = [c_double, c_double]
lib.integrated.restype = c_double

# Define initial conditions
x1 = 0  # Lower bound for x
x2 = 1  # Upper bound for x

plt.figure(figsize=(8, 6))

# Theoretical Plot for the upper half-circle
X = np.linspace(-1, 1, 5000)  # Full range for the circle
Y = np.sqrt(1 - X**2)  # y = sqrt(1 - x^2) for the upper semicircle
plt.plot(X, Y, label="$x^2 + y^2 = 1$", color='red')  # Circle equation

X = np.linspace(-1, 1, 5000)  # Full range for the circle
Y = -1*np.sqrt(1 - X**2)  # y = sqrt(1 - x^2) for the upper semicircle
plt.plot(X, Y, label="$x^2 + y^2 = 1$", color='red')  # Circle equation


# Shade the region between x1 and x2 under the semicircle
x_region = np.linspace(-1, 1, 3000)
y_region = np.sqrt(1 - x_region**2)
plt.fill_between(x_region, y_region, color='orange', alpha=0.3, label='Bounded Region')

x_region = np.linspace(-1, 1, 3000)
y_region = -np.sqrt(1 - x_region**2)
plt.fill_between(x_region, y_region, color='orange', alpha=0.3, label='Bounded Region')

# Call integrate to compute the area
Area = lib.integrated(c_double(x1), c_double(x2))
print("Area is", Area)

# Set labels, grid, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()

# Save the plot
plt.savefig("../figs/fig_circle.png")
plt.show()

