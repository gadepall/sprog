import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
parabola_line_lib = ctypes.CDLL("./de.so")

# Define argument and return types for the functions
parabola_line_lib.solveQuadratic.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                             ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
parabola_line_lib.solveQuadratic.restype = ctypes.c_int

parabola_line_lib.calculate_intersection_and_area.argtypes = [
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.c_double, ctypes.c_int
]
parabola_line_lib.calculate_intersection_and_area.restype = ctypes.c_int

# Variables for the function
x1 = ctypes.c_double()
y1 = ctypes.c_double()
x2 = ctypes.c_double()
y2 = ctypes.c_double()
area = ctypes.c_double()

# Parameters for the parabola and line
a = 1.0   # Parameter of the parabola y^2 = 4ax
n = 1000  # Number of subdivisions for numerical integration

# Call the C function
num_solutions = parabola_line_lib.calculate_intersection_and_area(
    ctypes.byref(x1), ctypes.byref(y1),
    ctypes.byref(x2), ctypes.byref(y2),
    ctypes.byref(area), a, n
)

# Prepare data for plotting the parabola and line
x_vals = np.linspace(0, 4 * a, 500)
parabola_y_vals = np.sqrt(4 * a * x_vals)
line_y_vals = x_vals

# Generate points for shading the area
if num_solutions == 2:
    x_points = np.linspace(x1.value, x2.value, 500)
    y_parabola = np.sqrt(4 * a * x_points)
    y_line = x_points
    plt.fill_between(x_points, y_line, y_parabola, where=(y_parabola >= y_line), color="lightblue", alpha=0.5)

# Plot the parabola
plt.plot(x_vals, parabola_y_vals, label="Parabola", color="blue")
plt.plot(x_vals, -parabola_y_vals, color="blue")

# Plot the line
plt.plot(x_vals, line_y_vals, label="Line", color="red")

# Highlight intersection points
if num_solutions == 2:
    plt.scatter([x1.value, x2.value], [y1.value, y2.value], color="green", label="Intersection Points")

# Add labels, legend, and title
plt.xlabel("x")
plt.ylabel("y")
plt.axis('equal')
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
plt.xlim(0, 4 * a)
plt.ylim(-4 * a, 4 * a)
plt.legend()
plt.grid()
plt.show()
