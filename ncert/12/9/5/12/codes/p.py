import ctypes
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# Load the shared library
lib = ctypes.CDLL("./c.so")

# Define the function signature for `points`
lib.points.argtypes = [
    ctypes.c_float,  # x_0
    ctypes.c_float,  # y_0
    ctypes.c_float,  # x_end
    ctypes.c_float,  # h
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # x_points
    np.ctypeslib.ndpointer(dtype=np.float32, ndim=1),  # y_points
    ctypes.c_int     # steps
]

# Parameters for simulation
x_0 = 1.0
y_0 = 1.0
x_end = 3.0
step_size = 0.001  # Reduced step size for higher accuracy
steps = int((x_end - x_0) / step_size) + 1

# Create numpy arrays to hold the points
x_points = np.zeros(steps, dtype=np.float32)
y_points = np.zeros(steps, dtype=np.float32)

# Call the `points` function from the C shared library
lib.points(x_0, y_0, x_end, step_size, x_points, y_points, steps)

# Define the implicit function for the theoretical solution
def implicit_function(y, x):
  
    return y + 2*x - 3* x * x * y 

# Generate theoretical values for y based on the implicit function
x_theory = np.linspace(1, 3, 500)
implicit_y = []

for xi in x_theory:
    yi_initial_guess = 1.0  # Initial guess for y
    yi_solution = fsolve(implicit_function, yi_initial_guess, args=(xi,))
    implicit_y.append(yi_solution[0])

# Convert implicit_y to a numpy array
implicit_y = np.array(implicit_y)

# Plot the results
plt.figure(figsize=(10, 6))

# Plot Euler's method results
plt.plot(x_points, y_points, 'ro-', markersize=2, linewidth=1, label="sim")

# Plot the theoretical solution
plt.plot(x_theory, implicit_y, 'b-', linewidth=2, label="theory")

# Add labels, title, grid, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True, linestyle="--")
plt.legend()

# Display the plot
plt.show()

