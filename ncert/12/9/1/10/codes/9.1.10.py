import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./9.1.10.so')

# Define argument and return types of the functions
lib.sol.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.sol.restype = None

# Function to run the solver
def solve_system(x0, y0, v0, r, n, h):
    # Create C-style variables
    x = ctypes.c_double(x0)
    y = ctypes.c_double(y0)
    v = ctypes.c_double(v0)
    
    # Call the shared function
    lib.sol(ctypes.byref(x), ctypes.byref(y), ctypes.byref(v), r, n, h)
    
    # Return the results
    return x.value, y.value, v.value

# Initialize parameters
x0, y0, v0 = 0.0, 1.0, 1.0  # Initial conditions
r = 0.1  # Some constant value
n = 1000  # Number of iterations
h = 0.01  # Step size

# Store data for plotting
x_vals = []
y_vals = []

# Iterate to compute and collect data points
for i in range(n):
    x, y, v = solve_system(x0, y0, v0, r, i, h)
    x_vals.append(x)
    y_vals.append(y)

# Plot the results
plt.plot(x_vals, y_vals, label='Sim')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.savefig("9.1.10.png")
plt.show()

