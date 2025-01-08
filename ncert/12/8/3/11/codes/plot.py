import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared object file
trapezoid = ctypes.CDLL('./solver.so')

# Define function prototypes for the C functions
trapezoid.area.restype = ctypes.c_double
trapezoid.area.argtypes = [
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),
    ctypes.c_double, ctypes.c_double, ctypes.c_double
]

trapezoid.getpoints.restype = ctypes.POINTER(ctypes.c_double)
trapezoid.getpoints.argtypes = [
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),
    ctypes.c_double, ctypes.c_double, ctypes.c_double
]

# Define the function y = 1 - x in Python
@ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
def given_function(x):
    return 1 - x

# Define the derivative of the function, y' = -1
@ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
def derivative_of_func(x):
    return -1

# Parameters
lower_limit = 0.0
upper_limit = 1.0
stepsize = 0.01
no_points = int((upper_limit - lower_limit) / stepsize) + 1

# Calculate area using the C function
total_area = trapezoid.area(given_function, derivative_of_func, lower_limit, upper_limit, stepsize)
print(f"Approximated area under the curve: {total_area:.6f}")

# Get points using the C function
points_ptr = trapezoid.getpoints(given_function, lower_limit, upper_limit, stepsize)

# Convert C array to Python array
x_values = np.linspace(lower_limit, upper_limit, no_points)
y_values = np.array([points_ptr[i] for i in range(no_points)])

# Generate points for the diamond shape
x_values2 = np.linspace(-1, 1, 500)
y_positive = 1 - np.abs(x_values2)  # For the top half of the diamond
y_negative = -y_positive  # For the bottom half of the diamond

# Combine points to form the complete diamond
x_combined = np.concatenate([x_values2, x_values2[::-1]])
y_combined = np.concatenate([y_positive, y_negative[::-1]])

# Plot the graph
plt.figure(figsize=(8, 8))
plt.plot(x_combined, y_combined, label=f'|x| + |y| = {1}', color='green')
plt.plot(x_values, y_values, label='y = 1 - x', color='blue')
plt.fill_between(x_values, y_values, alpha=0.2, color='green', label=f'Area calculated to be {total_area:.6f}')
plt.xlabel('x')
plt.ylabel('y')
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')
plt.axis([-1.5, 1.5, -1.5, 1.5])  # Adjust limits to show the full diamond
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")
plt.show()
