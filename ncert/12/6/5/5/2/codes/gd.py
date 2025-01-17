import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library (make sure it's compiled correctly)
gradient_lib = ctypes.CDLL('./gd.so')

# Set the return type of the functions in the shared library
gradient_lib.find_maximum_x.restype = ctypes.c_double
gradient_lib.find_minimum_x.restype = ctypes.c_double

# Call the C function to get the values of x where the function is maximized and minimized
optimal_max_x = gradient_lib.find_maximum_x()
optimal_min_x = gradient_lib.find_minimum_x()
# Define the function f(x) = sin(x) + cos(x)
def function_f(x):
    return np.sin(x) + np.cos(x)

# Calculate the maximum and minimum values of the function
max_value = function_f(optimal_max_x)
min_value = function_f(optimal_min_x)

# Generate x values for plotting over the domain [0, pi]
x_values = np.linspace(0, np.pi, 500)
y_values = function_f(x_values)

# Plot the function
plt.figure(figsize=(8, 6))  # Set figure size for better visual appeal
plt.plot(x_values, y_values, label=r'$f(x) = \sin(x) + \cos(x)$', color='#1f77b4', linewidth=2)  # Use a solid line with a nice color

# Add markers for the maximum and minimum points with more stylish appearance
plt.scatter([optimal_max_x], [function_f(optimal_max_x)], color='#ff6347', s=100, zorder=5, edgecolor='black', marker='o', label='Maximum Point')
plt.scatter([optimal_min_x], [function_f(optimal_min_x)], color='#32cd32', s=100, zorder=5, edgecolor='black', marker='o', label='Minimum Point')

# Titles and labels
plt.xlabel('x', fontsize=14)
plt.ylabel('f(x)', fontsize=14)

# Customize the grid for better visual effect
plt.grid(True, linestyle='--', color='gray', alpha=0.7)
plt.legend(frameon=False, fontsize=12)
plt.tight_layout()
plt.show()
