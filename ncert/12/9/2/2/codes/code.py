import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Loading the shared library
euler = ctypes.CDLL('./code.so')

# Defining arguments and return types of the C function
euler.euler_method.argtypes = [
    ctypes.c_double,   # x0
    ctypes.c_double,   # xn
    ctypes.c_double,   # h
    ctypes.POINTER(ctypes.c_double),  # x_values
    ctypes.POINTER(ctypes.c_double),  # y_values
    ctypes.c_int       # num_points
]

# Parameters
x0 = -4.0  # Start of x range
xn = 4.0   # End of x range
h = 0.01   # Step size
num_points = int((xn - x0) / h) + 1

# Memory allocation for arrays
x_values = (ctypes.c_double * num_points)()
y_values = (ctypes.c_double * num_points)()

# Example: y = x^2 + 2x + C
C = 1.0  # Example constant value

def python_euler_method(x0, xn, h, x_values, y_values, num_points):
    x = x0
    for i in range(num_points):
        x_values[i] = x
        y_values[i] = x**2 + 2*x + C
        x += h

# Call the Python function to fill the arrays
python_euler_method(x0, xn, h, x_values, y_values, num_points)

# Convert results to NumPy arrays for easier plotting
x_values = np.array(x_values)
y_values = np.array(y_values)

# Plot the results
plt.plot(x_values, y_values, label='Theory', color='b')
plt.plot(x_values, y_values, label="Sim", linestyle='--', color='r')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Function and Euler Method Overlap')
plt.legend()
plt.grid()
plt.savefig('fig.png')
plt.show()
