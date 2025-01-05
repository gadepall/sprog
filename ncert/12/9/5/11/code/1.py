import ctypes
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve
# Load the shared library
lib = ctypes.CDLL("./c.so")

# Define function signature
lib.points.argtypes = [
    ctypes.c_float,  # x_0
    ctypes.c_float,  # y_0
    ctypes.c_float,  # x_end
    ctypes.c_float,  # h
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # x_points
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # y_points
    ctypes.c_int     # steps
]

# Parameters
x_0 = 1.0
y_0 = 1.0
x_end = 3.0
step_size = 0.004
steps = int((x_end - x_0) / step_size) + 1

# Create numpy arrays to hold the points
x_points = np.zeros(steps, dtype=np.float32)
y_points = np.zeros(steps, dtype=np.float32)

# Call the function
lib.points(x_0, y_0, x_end, step_size, x_points, y_points, steps)


# Create np.linspace for x from 1 to 3, divided into 500 points
x = np.linspace(1, 3, 500)

# Initialize y array with the same length as x
y = np.zeros_like(x)

# Set the first value of y
y[0] = y_0

# Step size
h = step_size



# Define the implicit function  solved theoretically
def implicit_function(y, x):
    return 0.5 * np.log(y**2 / x**2 + 1) + np.arctan(y / x) + np.log(x) - (np.pi / 4 + 0.5 * np.log(2))

# Solve the implicit function for each x to find corresponding y
implicit_y = []
for xi in x:
    yi_initial_guess = 1  # Initial guess for y
    yi_solution = fsolve(implicit_function, yi_initial_guess, args=(xi,))
    implicit_y.append(yi_solution[0])

# Convert implicit_y to a numpy array
implicit_y = np.array(implicit_y)


# Plot the points
plt.figure(figsize=(10, 6))
plt.plot(x_points, y_points, 'ro-', markersize=2, linewidth=4, label="sim")
plt.plot(x, implicit_y, label='theory', color='blue')
# Add labels, title, grid, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True, which="both", linestyle="--", linewidth=1.0)
plt.legend()

# Save the plot to a PDF file and display
plt.show()
