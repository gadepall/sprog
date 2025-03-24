import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
# Ensure the compiled C file (e.g., plot_abs_function.so) is in the same directory
gradient_lib = ctypes.CDLL('./c.so')

# Call the C function to get the value of x where y is minimized
gradient_lib.find_minimum_x.restype = ctypes.c_double
optimal_x = gradient_lib.find_minimum_x()

print(f"The value of x that minimizes the function is: {optimal_x}")

# Define the function f(x) = |x + 2| - 1
def function_y(x):
    return np.abs(x + 2) - 1

# Calculate the minimum value of the function
value = function_y(optimal_x)
print(f"The minimum value of the function is: {value}")

# Generate x values for plotting
x_values = np.linspace(-5, 1, 500)
y_values = function_y(x_values)

# Plot the function
plt.figure(figsize=(8, 6))  # Set figure size for better visual appeal
plt.plot(x_values, y_values, label='f(x) = |x + 2| - 1', color='#1f77b4', linewidth=2)  # Use a solid line with a nice color

# Add markers for the minimum point with a more stylish appearance
plt.scatter([optimal_x], [function_y(optimal_x)], color='#ff6347', s=100, zorder=5, edgecolor='black', marker='o', label='Minimum Point')

# Titles and labels
plt.title('Plot of f(x) = |x + 2| - 1', fontsize=16)
plt.xlabel('x', fontsize=14)
plt.ylabel('f(x)', fontsize=14)

# Customize the grid for better visual effect
plt.grid(True, linestyle='--', color='gray', alpha=0.7)

# Add a legend with enhanced style
plt.legend(frameon=False, fontsize=12)

# Apply tighter layout for better spacing
plt.tight_layout()

# Show the plot
plt.show()

