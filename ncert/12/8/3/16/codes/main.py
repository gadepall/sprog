import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
curve_lib = ctypes.CDLL("./main.so")

# Define argument and return types for the functions
curve_lib.calculate_area.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.POINTER(ctypes.c_double)
]
curve_lib.calculate_area.restype = ctypes.c_double

# Variables for the function
area = ctypes.c_double()

# Parameters for the curve and integration limits
x_start = -2.0  # Start x-coordinate
x_end = 1.0     # End x-coordinate
n = 1000        # Number of subdivisions for numerical integration

# Call the C function to calculate the area
area_value = curve_lib.calculate_area(x_start, x_end, n, ctypes.byref(area))
# Prepare data for plotting
x_values = np.linspace(x_start, x_end, 500)  # X values from -2 to 1
y_values = x_values ** 3  # Curve equation y = x^3

# Plot the curve
plt.plot(x_values, y_values, label="Curve (y = x^3)", color="blue")

# Plot the integration limits x = -2 and x = 1
plt.axvline(x=-2, color='green', linestyle="--", label="x = -2")
plt.axvline(x=1, color='red', linestyle="--", label="x = 1")

# Shading the area under the curve between x_start and x_end
x_shaded = np.linspace(x_start, x_end, 500)  # x-values from -2 to 1
y_shaded = x_shaded ** 3  # y-values on the curve y = x^3

# Shade under the curve
plt.fill_between(x_shaded, y_shaded, color="lightblue", alpha=0.5, label="Shaded Area")

# Add labels, legend, and title
plt.xlabel("x")
plt.ylabel("y")
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
plt.xlim(x_start - 1, x_end + 1)
plt.ylim(min(y_values) - 1, max(y_values) + 1)
plt.legend()
plt.grid()
plt.axis("equal")
# Annotate x = -2 and x = 1 on the plot
plt.text(-2, 0, 'x = -2', color='green', verticalalignment='bottom')
plt.text(1, 0, 'x = 1', color='red', verticalalignment='bottom')

# Show the plot
plt.show()

