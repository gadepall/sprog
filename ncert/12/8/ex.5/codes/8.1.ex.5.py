import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# lib stores the shared object file . 
lib = ctypes.CDLL('./8.1.ex.5.so')

# Define argument and return types for generate_points

lib.calc_area.restype = ctypes.c_double
#result2 variable contains area calculated by using difference equation by using trapezoidal rule
result2 = lib.calc_area()
#theoretical area
area = 9*np.sqrt(7)/4 + 12*(math.asin(np.sqrt(7)/4))
print("Theoretical area is: ",area)
print("Area using trapezoidal rule: ",result2)
a = 4  # Semi-major axis (sqrt(16))
b = 3  # Semi-minor axis (sqrt(9))

# Generate points for the ellipse
theta = np.linspace(0, 2 * np.pi, 500)
x_ellipse = a * np.cos(theta)
y_ellipse = b * np.sin(theta)

# Define the vertical lines x = 0 and x = sqrt(7)
x_vertical1 = 0
x_vertical2 = np.sqrt(7)

# Generate the y-range for shading
y_range = np.linspace(-b, b, 500)

# Set up the plot
plt.figure(figsize=(8, 6))
plt.plot(x_ellipse, y_ellipse, label="Ellipse: $\\frac{x^2}{16} + \\frac{y^2}{9} = 1$", color="blue")
plt.axvline(x_vertical1, color="red", linestyle="--", label="x = 0")
plt.axvline(x_vertical2, color="green", linestyle="--", label="x = $\\sqrt{7}$")

# Highlight the bounded area
x_line = np.sqrt(7)
x_fill = np.linspace(0, x_line, 500)
y_fill_upper = b * np.sqrt(1 - (x_fill**2 / a**2))
y_fill_lower = -b * np.sqrt(1 - (x_fill**2 / a**2))

plt.fill_between(x_fill, y_fill_upper, y_fill_lower, color="orange", alpha=0.4, label="Enclosed Area")
# Labels and legend
plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.axhline(0, color="black", linewidth=0.5)
plt.axvline(0, color="black", linewidth=0.5)
plt.grid(alpha=0.5)
plt.legend(loc = "upper right")
plt.axis("equal")

# Display the plot
plt.show()



