import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL("./de.so")

# Define function signature
lib.points.argtypes = [
    ctypes.c_float,  # x_0
    ctypes.c_float,  # y_0
    ctypes.c_float,  # y_end
    ctypes.c_float,  # h
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # x_points
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # y_points
    ctypes.c_int     # steps
]

# Parameters
x_0 = 0.0  # Initial x value
y_0 = 1.0  # Initial y value
y_end = 5.0  # Final y value
step_size = 0.001  # Step size for Euler's method
steps = int((y_end - y_0) / step_size) + 1  # Calculate number of steps

# Create numpy arrays to hold the points
x_points = np.zeros(steps, dtype=np.float32)
y_points = np.zeros(steps, dtype=np.float32)

# Call the C function to compute points using Euler's method
lib.points(x_0, y_0, y_end, step_size, x_points, y_points, steps)

# Interpolate the smooth curve
x_smooth = np.linspace(min(x_points), max(x_points), 500)
y_smooth = np.interp(x_smooth, x_points, y_points)

# Plot the computed points and the smooth curve
plt.figure(figsize=(10, 6))
plt.plot(x_points, y_points, 'ro', markersize=2, label="sim")
plt.plot(x_smooth, y_smooth, label="theory", color='blue')

# Add labels, title, grid, and legend
plt.xlabel("X - Axis")
plt.ylabel("Y - Axis")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()

# Show the plot
plt.show()
