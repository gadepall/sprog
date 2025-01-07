import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL("./c.so")

# Define the function signature for points
lib.points.argtypes = [
    ctypes.c_float,  # x_0
    ctypes.c_float,  # y_0
    ctypes.c_float,  # x_end
    ctypes.c_float,  # h
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # x_points
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # y_points
    ctypes.c_int     # stepsclass 12 differential equations
]

# Parameters for simulation
x_0 = 0.0  # Initial condition for x
y_0 = 2.0  # Initial condition for y
x_end = 1.0  # Final value of x
step_size = 0.001  # Reduced step size for higher accuracy
steps = int((x_end - x_0) / step_size) + 1

# Create numpy arrays to hold the points
x_points = np.zeros(steps, dtype=np.float32)
y_points = np.zeros(steps, dtype=np.float32)

# Call the points function from the C shared library
lib.points(x_0, y_0, x_end, step_size, x_points, y_points, steps)

# Define the theoretical solution with C = -2
def theoretical_solution(x):
    return (-x + 4 - 2* np.exp(x))  # C = -2

# Generate theoretical values for y
x_theory = np.linspace(x_0, x_end, 1000)
y_theory = theoretical_solution(x_theory)

# Plot the results
plt.figure(figsize=(10, 6))

# Plot Euler's method results
plt.plot(x_points, y_points, 'ro-', markersize=2, linewidth=4, label="sim")

# Plot the theoretical solution
plt.plot(x_theory, y_theory, 'b-', linewidth=2, label="theory")

# Add labels, title, grid, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True, linestyle="--")
plt.legend()

# Display the plot
plt.show()

