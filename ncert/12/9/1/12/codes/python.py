import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./func.so')

# Define constants
N = 99
x0, xf = 1, 10  # Start from x = 1 to avoid division by zero
h = 0.1
steps = int((xf - x0) / h) + 1

# Allocate memory for the matrix (2 rows x N columns)
matrix = (ctypes.POINTER(ctypes.c_double) * 2)()
matrix[0] = (ctypes.c_double * N)()
matrix[1] = (ctypes.c_double * N)()

# Initial conditions
init_condition = (ctypes.c_double * 2)(1.0, 0.0)  # Example: y(1) = 1, y'(1) = 0

# Function prototypes
lib.euler_method_2nd_order.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # matrix
    ctypes.c_double,                                 # step size (h)
    ctypes.c_int,                                    # x0
    ctypes.c_int,                                    # xf
    ctypes.POINTER(ctypes.c_double)                  # initial conditions
]
lib.euler_method_2nd_order.restype = None

# Call the Euler method function
lib.euler_method_2nd_order(matrix, h, x0, xf, init_condition)

# Extract values for plotting
x_values = [x0 + i * h for i in range(steps)]
y_values = [matrix[0][i] for i in range(steps)]

# Plot the results
plt.plot(x_values, y_values, color='blue', marker='o', linestyle='-', label='Numerical Solution')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Solution to $2x^2 \\frac{d^2y}{dx^2} - 3 \\frac{dy}{dx} + y = 0$')
plt.grid(True)
plt.legend()
plt.savefig("../figs/fig2.png")
plt.show()

