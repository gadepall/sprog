import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
parabola_lib = ctypes.CDLL("./q3.so")

# Define argument and return types for the functions
parabola_lib.calculate_area.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.POINTER(ctypes.c_double)
]
parabola_lib.calculate_area.restype = ctypes.c_double

# Variables for the function
area = ctypes.c_double()

# Parameters for the parabola and integration limits
y1 = 2.0  # Start y-coordinate (y = 2)
y2 = 4.0  # End y-coordinate (y = 4)
n = 1000   # Number of subdivisions for numerical integration

# Call the C function to calculate the area
area_value = parabola_lib.calculate_area(y1, y2, n, ctypes.byref(area))

# Prepare data for plotting
x_values = np.linspace(0, np.sqrt(4 * y2), 500)  # X values from 0 to sqrt(4 * 4)
y_values = (x_values ** 2) / 4.0  # Parabola equation y = x^2 / 4

# Plot the parabola
plt.plot(x_values, y_values, label="Parabola (y = x^2 / 4)", color="blue")

# Plot the horizontal lines y = 2 and y = 4 for shading the area
plt.axhline(y=2, color='green', linestyle="--", label="y = 2")
plt.axhline(y=4, color='red', linestyle="--", label="y = 4")

# Shading the area between the parabola and horizontal lines y = 2 and y = 4
y_shaded = np.linspace(y1, y2, 500)  # y-values from 2 to 4
x_shaded_left = np.sqrt(4 * y_shaded)  # Left side of the parabola
x_shaded_right = -np.sqrt(4 * y_shaded)  # Right side of the parabola (not used in first quadrant)

# Shade between y1 and y2 (between the parabola and horizontal lines)
plt.fill_betweenx(y_shaded, 0, x_shaded_left, color="lightblue", alpha=0.5, label="Shaded Area")

# Add labels, legend, and title
plt.xlabel("x")
plt.ylabel("y")
plt.title(f"Area under the parabola from y = 2 to y = 4: {area_value:.5f} square units")
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
plt.xlim(0, np.sqrt(4 * y2) + 1)
plt.ylim(0, 5)
plt.legend()
plt.grid()

# Annotate y = 2 and y = 4 on the plot
plt.text(np.sqrt(4 * y2) - 0.5, 2, 'y = 2', color='green', verticalalignment='bottom')
plt.text(np.sqrt(4 * y2) - 0.5, 4, 'y = 4', color='red', verticalalignment='top')

# Save the plot
plt.savefig("../figs/fig.png")

# Show the plot
plt.show()

