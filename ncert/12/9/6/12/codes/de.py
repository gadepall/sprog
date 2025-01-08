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
x_0 = 1.0
y_0 = 1.0
y_end = 5.0  # Final y value
step_size = 0.001
steps = int((y_end - y_0) / step_size) + 1

# Create numpy arrays to hold the points
x_points = np.zeros(steps, dtype=np.float32)
y_points = np.zeros(steps, dtype=np.float32)

# Call the points function
lib.points(x_0, y_0, y_end, step_size, x_points, y_points, steps)

# Generate a smooth x-axis using np.linspace
x_smooth = np.linspace(x_0, y_end, 500)
y_smooth = np.interp(x_smooth, x_points, y_points)

# Plot the points and the smooth curve
plt.figure(figsize=(10, 6))
plt.plot(x_points, y_points, 'ro', markersize=2, label="sim")
plt.plot(x_smooth, y_smooth, label="theory", color='blue')

# Add labels, title, grid, and legend
plt.xlabel("X - Axis")
plt.ylabel("Y - Axis")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.show()
