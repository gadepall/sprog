import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
circle_line_lib = ctypes.CDLL("./circle.so")

# Define argument and return types for the functions
circle_line_lib.solveQuadratic.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                           ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
circle_line_lib.solveQuadratic.restype = ctypes.c_int

circle_line_lib.calculate_intersection_and_area.argtypes = [
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.c_double, ctypes.c_double,
    ctypes.c_double, ctypes.c_int
]
circle_line_lib.calculate_intersection_and_area.restype = ctypes.c_int

# Variables for the function
x1 = ctypes.c_double()
y1 = ctypes.c_double()
x2 = ctypes.c_double()
y2 = ctypes.c_double()
area = ctypes.c_double()

# Parameters for the line and circle
m = -1.0  # Slope of the line
c = 2.0   # Intercept of the line
radius = 2.0  # Radius of the circle
n = 1000  # Number of subdivisions for numerical integration

# Call the C function
num_solutions = circle_line_lib.calculate_intersection_and_area(
    ctypes.byref(x1), ctypes.byref(y1),
    ctypes.byref(x2), ctypes.byref(y2),
    ctypes.byref(area), m, c, radius, n
)

# Prepare data for plotting
theta = np.linspace(0, 2 * np.pi, 500)
circle_x = radius * np.cos(theta)
circle_y = radius * np.sin(theta)

line_x = np.linspace(-radius - 1, radius + 1, 500)
line_y = m * line_x + c

# Generate points for shading the area
if num_solutions == 2:
    x_points = np.linspace(x1.value, x2.value, 500)
    y_circle = np.sqrt(radius**2 - x_points**2)
    y_line = m * x_points + c
    plt.fill_between(x_points, y_line, y_circle, where=(y_circle >= y_line), color="lightblue", alpha=0.5)

# Plot the circle
plt.plot(circle_x, circle_y, label="Circle")

# Plot the line
plt.plot(line_x, line_y, label=f"Line", color="red")

# Highlight intersection points
if num_solutions == 2:
    plt.scatter([x1.value, x2.value], [y1.value, y2.value], color="green", label="Intersection Points")

# Add labels, legend, and title
plt.xlabel("x")
plt.ylabel("y")
plt.axis('equal')
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
plt.xlim(-3, 3)
plt.ylim(-3,3)
plt.legend()
plt.grid()
plt.savefig("../figs/fig.png")
