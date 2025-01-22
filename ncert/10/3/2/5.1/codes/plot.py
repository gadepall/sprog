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
line1 = [1, -1, -4]  # Example: 5x - 4y + 8 = 0
line2 = [1, 1, -72]  # Example: 7x + 6y - 9 = 0

# Extract coefficients
a1, b1, c1 = line1
a2, b2, c2 = line2

# Call the C function
result = lib.find_intersection(a1, b1, c1, a2, b2, c2)

# Debugging: Print results from the C function
print("C function result:")
print(f"Exists: {result.exists}, Coincident: {result.coincident}, Parallel: {result.parallel}")
if result.exists:
    print(f"Intersection at: ({result.x}, {result.y})")

# Interpret results and print output
if result.coincident:
    print("The lines are coincident (identical).")
elif result.parallel:
    print("The lines are parallel (no intersection).")
elif result.exists:
    print(f"The lines intersect at: ({result.x:.6f}, {result.y:.6f})")
else:
    print("Unexpected case. Please check input or logic.")

# Plotting
x = np.linspace(-10, 50, 400)  # X range for plotting

# Plot line 1
if b1 != 0:  # Regular line
    y1 = (-a1 * x - c1) / b1
    plt.plot(x, y1, label="Line 1: {}x + {}y + {} = 0".format(a1, b1, c1), color='blue')
else:  # Vertical line
    x1 = -c1 / a1
    plt.axvline(x=x1, color='blue', linestyle='--', label="Line 1: {}x + {}y + {} = 0".format(a1, b1, c1))

# Plot line 2
if b2 != 0:  # Regular line
    y2 = (-a2 * x - c2) / b2
    plt.plot(x, y2, label="Line 2: {}x + {}y + {} = 0".format(a2, b2, c2), color='red')
else:  # Vertical line
    x2 = -c2 / a2
    plt.axvline(x=x2, color='red', linestyle='--', label="Line 2: {}x + {}y + {} = 0".format(a2, b2, c2))

# Plot intersection point (if it exists)
if result.exists:
	plt.scatter(result.x, result.y, color="black", zorder=5,
		        label="Intersection: ({:.2f}, {:.2f})".format(result.x, result.y))

# Add labels, legend, and grid
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.title("Line Intersection")
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()

# Save and show the plot
plt.savefig('../figs/fig.png')
plt.show()

