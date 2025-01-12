import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./q4.so')

# Call the C function to get the value of x where y is maximized
gradient_lib.find_maximum_x.restype = ctypes.c_double
optimal_x = gradient_lib.find_maximum_x()

print(f"The value of x that maximizes the function is: {optimal_x}")

# Define the function y = -|x + 1| + 3
def function_y(x):
    return -np.abs(x + 1) + 3

# Calculate the maximum value of the function
value = function_y(optimal_x)
print(f"The maximum value of the function is: {value}")

# Generate x values for plotting
x_values = np.linspace(-3, 3, 500)
y_values = function_y(x_values)

# Plot the function
plt.figure(figsize=(8, 6))  # Set figure size for better visual appeal
plt.plot(x_values, y_values, label='y = -|x + 1| + 3', color='#1f77b4', linewidth=2)  # Use a solid line with a nice color

# Add markers for the maximum point with more stylish appearance
plt.scatter([optimal_x], [function_y(optimal_x)], color='#ff6347', s=100, zorder=5, edgecolor='black', marker='o', label='Maximum Point')

# Titles and labels
plt.xlabel('x', fontsize=14)
plt.ylabel('y', fontsize=14)

# Customize the grid for better visual effect
plt.grid(True, linestyle='--', color='gray', alpha=0.7)

# Add a legend with enhanced style
plt.legend(frameon=False, fontsize=12)

# Apply tighter layout for better spacing
plt.tight_layout()

# Show the plot
plt.show()

