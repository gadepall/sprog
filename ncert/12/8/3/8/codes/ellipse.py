import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./ellipse.so')

# Define argument and return types
lib.calculate_intersection_and_area.argtypes = [
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int
]

lib.calculate_intersection_and_area.restype = ctypes.c_int

# User inputs for ellipse and line
a, b = 3.0, 2.0  # Semi-major and semi-minor axes
m, c = -2/3, 2.0  # Line slope and intercept
n = 1000          # Number of trapezoids

# Variables to hold results
x1, y1, x2, y2, area = ctypes.c_double(), ctypes.c_double(), ctypes.c_double(), ctypes.c_double(), ctypes.c_double()

# Call the C function
num_solutions = lib.calculate_intersection_and_area(
    ctypes.byref(x1), ctypes.byref(y1),
    ctypes.byref(x2), ctypes.byref(y2),
    ctypes.byref(area),
    a, b, m, c, n
)

# Process results
if num_solutions == 0:
    print("No intersection points.")
else:
    x1, y1, x2, y2, area = x1.value, y1.value, x2.value, y2.value, abs(area.value)
    print(area)
    # Plot the ellipse
    theta = np.linspace(0, 2 * np.pi, 1000)
    x_ellipse = a * np.cos(theta)
    y_ellipse = b * np.sin(theta)

    # Plot the line
    x_line = np.linspace(-a - 1, a + 1, 1000)
    y_line = m * x_line + c

    # Plot the shaded region
    x_shade = np.linspace(x1, x2, 1000)
    y_ellipse_upper = np.sqrt(b**2 * (1 - x_shade**2 / a**2))
    y_shade_line = m * x_shade + c

    plt.fill_between(x_shade, y_shade_line, y_ellipse_upper, where=(y_shade_line < y_ellipse_upper),
                     color='blue', alpha=0.3, label=f'Area = {area:.2f}')

    # Plot the ellipse, line, and intersection points
    plt.plot(x_ellipse, y_ellipse, label="Ellipse: x²/a² + y²/b² = 1")
    plt.plot(x_line, y_line, label="Line: y = mx + c", color="orange")
    plt.scatter([x1, x2], [y1, y2], color="red", label="Intersection Points")

    # Label the intersection points
    plt.text(x1, y1, f"P1({x1:.2f}, {y1:.2f})", fontsize=10, color="red", ha='right', va='bottom')
    plt.text(x2, y2+0.3, f"P2({x2:.2f}, {y2:.2f})", fontsize=10, color="red", ha='left', va='top')

    # Customize plot
    plt.legend()
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)
    plt.axis('equal')
    plt.savefig('../figs/fig.png')
    plt.show()

