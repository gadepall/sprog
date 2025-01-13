import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./c.so')

# Call the C function to get the value of x where y is minimized
gradient_lib.find_minimum_x.restype = ctypes.c_double
optimal_x = gradient_lib.find_minimum_x()

print(f"The value of x that minimizes the function is: {optimal_x}")

# Define the function y = (2x - 1)^2 + 3
def function_y(x):
    return (2*x-1)**2+3
value = function_y(optimal_x)
print(f"The value of minimum value of the function is : {value}")
# Generate x values for plotting
x_values = np.linspace(-3, 3, 500)
y_values = function_y(x_values)

# Plot the function
plt.plot(x_values, y_values, label='y = (2x - 1)^2 + 3', color='blue')
plt.scatter([optimal_x], [function_y(optimal_x)], color='red', label='Minimum Point')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()
plt.show()

