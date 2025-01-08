import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library
gen = ctypes.CDLL('./points.so')

# Define argument and return types for generate_points
gen.generate_points.argtypes = [
    ctypes.c_double, ctypes.c_double,
    ctypes.POINTER(ctypes.c_double), ctypes.c_int
]
gen.generate_points.restype = None

# Define argument and return types for calc_area
gen.calc_area.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.POINTER(ctypes.c_double)
]
gen.calc_area.restype = None

# Number of points
num_points = 4000
a, b = 2, 3

# Create a NumPy array for points
points = np.zeros((num_points * 2, 2), dtype=np.double)

# Call generate_points
gen.generate_points(
    a, b,
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Calculate area
res = ctypes.c_double(0.0)
gen.calc_area(
    a, b, num_points, ctypes.byref(res)
)

print(f"Theoretical Area: {6 * math.pi}")
print(f"Computational result: {res.value}")

# Plotting the ellipse
fig = plt.figure()
ax = plt.gca()
plt.plot(points[:, 0], points[:, 1], c='r', label="ellipse")
ax.set_xlim(-a, a)
ax.set_ylim(-b, b)
plt.legend(loc="best")
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.savefig('../figs/fig.png')
plt.show()

# No need to call free_ptr for NumPy arrays or ctypes.c_double in Python

