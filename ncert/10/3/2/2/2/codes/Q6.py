import ctypes
import matplotlib.pyplot as plt
import numpy as np

# Load the shared library
lib = ctypes.CDLL('./line_intersection.so')

# Define the structure for the intersection point
class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_double),
                ("y", ctypes.c_double),
                ("exists", ctypes.c_int),
                ("coincident", ctypes.c_int),
                ("parallel", ctypes.c_int)]

# Set the return type for the C function
lib.find_intersection.restype = Point

# Input line coefficients: ax + by + c = 0
line1 = [9, 3, -12]  # Example: x - y - 2 = 0
line2 = [18, 6, -24]   # Example: x - 2 = 0 (vertical line)

# Call the C function
a1, b1, c1 = line1
a2, b2, c2 = line2
result = lib.find_intersection(a1, b1, c1, a2, b2, c2)

# Handle the cases
if result.coincident:
    print("The lines are coincident (identical).")
elif result.parallel:
    print("The lines are parallel (no intersection).")
elif result.exists:
    print(f"Intersection at: ({result.x}, {result.y})")

# Plotting
x = np.linspace(-10, 10, 400)  # X range for plotting

# Plot line 1
if b1 != 0:  # Regular line
    y1 = (-a1 * x - c1) / b1
    plt.plot(x, y1, label="Line 1: {}x + {}y + {} = 0".format(a1, b1, c1))
else:  # Vertical line
    x1 = -c1 / a1
    plt.axvline(x=x1, color='blue', linestyle='--', label="Line 1: {}x + {}y + {} = 0".format(a1, b1, c1))

# Plot line 2
if b2 != 0:  # Regular line
    y2 = (-a2 * x - c2) / b2
    plt.plot(x, y2, label="Line 2: {}x + {}y + {} = 0".format(a2, b2, c2))
else:  # Vertical line
    x2 = -c2 / a2
    plt.axvline(x=x2, color='red', linestyle='--', label="Line 2: {}x + {}y + {} = 0".format(a2, b2, c2))

# Plot intersection point
if result.exists:
    plt.scatter(result.x, result.y, color="black", label="Intersection: ({:.2f}, {:.2f})".format(result.x, result.y))

# Add labels and legend
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.title("Line Intersection")
plt.grid()
plt.legend()
plt.savefig('../figs/fig.png')
plt.show()
