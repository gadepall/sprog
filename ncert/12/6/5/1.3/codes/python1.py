import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./code.so')

# Call the C function to get the value of x where y is maximized
gradient_lib.find_maximum_x.restype = ctypes.c_double
optimal_x = gradient_lib.find_maximum_x()

print(f"The value of x that maximizes the function is: {optimal_x}")

# Define the function y = -(x - 1)^2 + 10
def function_y(x):
    return -(x - 1)**2 + 10

# Calculate the maximum value of the function
value = function_y(optimal_x)
print(f"The maximum value of the function is: {value}")

# Generate x values for plotting
x_values = np.linspace(-3, 3, 500)
y_values = function_y(x_values)

# Plot the function
plt.plot(x_values, y_values, label='y = -(x - 1)^2 + 10', color='blue')
plt.scatter([optimal_x], [function_y(optimal_x)], color='red', label='Maximum Point')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()
plt.show()

