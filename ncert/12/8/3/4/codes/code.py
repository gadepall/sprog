import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./code.so')

# Define argument and return types for generate_points
lib.generate_points.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Pointer to the points array
    ctypes.c_int                     # Number of points
]
lib.generate_points.restype = None

# Define argument and return types for calc_integral
lib.calc_integral.argtypes = [
    ctypes.c_int,                     # Number of points
    ctypes.POINTER(ctypes.c_double)   # Pointer to store the result
]
lib.calc_integral.restype = None

# Number of points
num_points = 4000

# Create a NumPy array for points
points = np.zeros((num_points * 2, 2), dtype=np.double)

# Call generate_points
lib.generate_points(
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Calculate area
res = ctypes.c_double(0.0)
lib.calc_integral(
    num_points,
    ctypes.byref(res)
)

# Plotting the function
fig = plt.figure()
ax = plt.gca()
plt.plot(points[:, 0], points[:, 1], c='r', label="|x + 3|")
ax.set_xlim(-6, 0)
ax.set_ylim(0, 6)
plt.legend(loc="best")
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.savefig('fig.png')
plt.show()

