import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./cc.so')

# Call the C function to get the value of x where y is maximized
gradient_lib.find_maximum_x.restype = ctypes.c_double
optimal_x = gradient_lib.find_maximum_x()

print(f"The value of x that maximizes the function is: {optimal_x}")

# Define the function y = x * sqrt(1 - x)
def function_y(x):
    return x * np.sqrt(1 - x)

# Calculate the maximum value of the function
value = function_y(optimal_x)
print(f"The maximum value of the function is: {value}")

# Generate x values for plotting
x_values = np.linspace(0, 1, 500)
y_values = function_y(x_values)

# Plot the function
plt.plot(x_values, y_values, label='y = x * sqrt(1 - x)', color='blue')
plt.scatter([optimal_x], [function_y(optimal_x)], color='red', label='Maximum Point')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()
plt.show()

