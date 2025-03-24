import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL("./c.so")

# Define function signature for points
lib.points.argtypes = [
    ctypes.c_float,  # x_0
    ctypes.c_float,  # y_0
    ctypes.c_float,  # x_end
    ctypes.c_float,  # h
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # x_points
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # y_points
    ctypes.c_int     # steps
]

# Parameters for the range of x and step size
x_0 = 0.1
y_0 = (-2 / x_0) * (1 + 1 / np.log(x_0))  # Initial y value based on the equation
x_end = 0.997
step_size = 0.001
steps = int((x_end - x_0) / step_size) + 1

# Create numpy arrays to hold the calculated points
x_points = np.zeros(steps, dtype=np.float32)
y_points = np.zeros(steps, dtype=np.float32)

# Call the C function to populate x_points and y_points arrays
lib.points(x_0, y_0, x_end, step_size, x_points, y_points, steps)

# Generate a smooth x-axis using np.linspace for interpolation
x_smooth = np.linspace(x_0, x_end, 500)
y_smooth = np.interp(x_smooth, x_points, y_points)

# Plot the Euler's method points and the smooth theory curve
plt.figure(figsize=(10, 6))
plt.plot(x_points, y_points, 'ro', markersize=2, label="sim")
plt.plot(x_smooth, y_smooth, label="Theory", color='blue')

# Add labels, title, grid, and legend
plt.xlabel("X - Axis")
plt.ylabel("Y - Axis")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.show()

