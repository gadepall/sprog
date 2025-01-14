import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
max_lib = ctypes.CDLL('./max.so')

# Set return types for the shared library functions
max_lib.get_last_point.restype = ctypes.c_double
max_lib.get_last_point_1.restype = ctypes.c_double

# Call the functions to get the max and min points
last_point_max = max_lib.get_last_point()
last_point_min = max_lib.get_last_point_1()

# Parameters for the function f(x)
ab = 20

# Define f(x)
def f(x):
    return ab * np.sin(x) * (1 - np.cos(x))

# Generate x values for the plot
x_vals = np.linspace(0, 2 * np.pi, 500)
y_vals = f(x_vals)

# Plot the function
plt.plot(x_vals, y_vals, label=r'$f(x) = ab \sin{x} (1 - \cos{x})$')

# Plot the max point
plt.scatter(
    [last_point_max], [f(last_point_max)], 
    color='red', label=f'Max Point: x={last_point_max:.4f}'
)

# Plot the min point
plt.scatter(
    [last_point_min], [f(last_point_min)], 
    color='blue', label=f'Min Point: x={last_point_min:.4f}'
)

# Add labels, legend, and grid
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()

