import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_double

# Load the shared library
lib = CDLL("./func.so")

# Specify the argument and return types for the C function
lib.Area.argtypes = [c_double, c_double]
lib.Area.restype = c_double

# Define the two circles
def circle1(x):
    return np.sqrt(4 - x**2)

def circle2(x):
    return np.sqrt(4 - (x - 2)**2)

# Intersection points of the circles
x1 = 0  # Left intersection point
x2 = 2  # Right intersection point

# Generate data points for plotting
x_values_circle1 = np.linspace(-2, 2, 1000)
y_values_circle1_top = circle1(x_values_circle1)
y_values_circle1_bottom = -circle1(x_values_circle1)

x_values_circle2 = np.linspace(0, 4, 1000)
y_values_circle2_top = circle2(x_values_circle2)
y_values_circle2_bottom = -circle2(x_values_circle2)

# Plot the circles
plt.figure(figsize=(8, 6))
plt.plot(x_values_circle1, y_values_circle1_top, label=r"$x^2 + y^2 = 4$", color='red')
plt.plot(x_values_circle1, y_values_circle1_bottom, color='red')
plt.plot(x_values_circle2, y_values_circle2_top, label=r"$(x-2)^2 + y^2 = 4$", color='blue')
plt.plot(x_values_circle2, y_values_circle2_bottom, color='blue')

# Highlight the enclosed region
x_fill = np.linspace(x1, x2, 1000)
y_fill_top = np.minimum(circle1(x_fill), circle2(x_fill))
y_fill_bottom = np.maximum(-circle1(x_fill), -circle2(x_fill))
plt.fill_between(x_fill, y_fill_bottom, y_fill_top, color='orange', alpha=0.3, label="Enclosed Region")

# Call the C function to compute the area
enclosed_area = lib.Area(c_double(x1), c_double(x2))
print(f"Enclosed Area: {enclosed_area:.6f}")

# Add labels, legend, and grid
plt.xlabel("x")
plt.ylabel("y")
plt.axhline(0, color='black', linestyle='--', linewidth=0.5)
plt.axvline(0, color='black', linestyle='--', linewidth=0.5)
plt.legend()
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.title("Area Enclosed Between Two Circles")

# Save and show the plot
plt.savefig("../figure/fig.png")
plt.show()

