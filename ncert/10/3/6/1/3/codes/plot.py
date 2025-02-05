import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL("./func.so")

# Define the function signature
lib.findIntersection.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,  # Line 1 coefficients
    ctypes.c_float, ctypes.c_float, ctypes.c_float,  # Line 2 coefficients
    ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)  # Pointers to store x and y
]

def find_intersection(a1, b1, c1, a2, b2, c2):
    x = ctypes.c_float()
    y = ctypes.c_float()
    
    lib.findIntersection(a1, b1, c1, a2, b2, c2, ctypes.byref(x), ctypes.byref(y))
    return x.value, y.value

# Define two lines: ax + by = c
line1 = (4, 3, 14)  # 2x + 3y = 5
line2 = (3, -4, 23)  # 4x + y = 6

# Find intersection
x_int, y_int = find_intersection(*line1, *line2)

# Print intersection in Python
print(f"Intersection Point (Python): t = {x_int}, y = {y_int}")

# Generate line data
x_vals = np.linspace(-5, 10, 100)
y_vals1 = (line1[2] - line1[0] * x_vals) / line1[1]
y_vals2 = (line2[2] - line2[0] * x_vals) / line2[1]

# Plot the lines
plt.plot(x_vals, y_vals1, label=f"{line1[0]}t + {line1[1]}y = {line1[2]}", color="blue")
plt.plot(x_vals, y_vals2, label=f"{line2[0]}t + {line2[1]}y = {line2[2]}", color="red")

# Plot intersection
plt.scatter(x_int, y_int, color="green", zorder=3, label=f"Intersection ({x_int:.2f}, {y_int:.2f})")

# Labels and grid
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()
plt.grid()
plt.title("Intersection of Two Lines")
plt.savefig('../figs/fig.png')
plt.show()
