import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared library
dll = ctypes.CDLL('./points.so')

# Define argument types and return types for functions in the shared library
dll.diffEqPoints.argtypes = [ctypes.c_int, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float]
dll.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

# Initial conditions and parameters
n = 200  # number of points to compute
h = 0.01  # step size
x0 = 0.1  # initial x (start at a small positive value to avoid division by zero)
y1_0 = 1.0  # initial y1
y2_0 = 0.0  # initial y2

# Create a figure for plotting
plt.figure(figsize=(8, 6))

# Get the points from the C function
pts = dll.diffEqPoints(n, h, x0, y1_0, y2_0)

# Extract coordinates from the returned data (pointer to pointer)
coords = []
for i in range(n):
    pt = pts[i]  # Access individual point (pt[0] for x, pt[1] for y)
    coords.append(np.array([pt[0], pt[1]]))

coords_plot = np.array(coords)

# Plot the computed points
plt.scatter(coords_plot[:, 0], coords_plot[:, 1], marker=".", label="sim", color="royalblue")

# Free the memory
dll.freeMultiMem(pts, n)

# Customize the plot (axes, labels, grid)
plt.xlabel('x')
plt.ylabel('y')
plt.legend(loc='best')
plt.grid()
plt.axis('equal')

# Show the plot
plt.show()

