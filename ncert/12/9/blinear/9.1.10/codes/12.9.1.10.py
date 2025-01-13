import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library (ensure the path to your shared library is correct)
lib = ctypes.CDLL('./12.9.1.10.so')

# Define argument and return types for the C functions
lib.euler_method.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                             ctypes.POINTER(ctypes.c_double), ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.euler_method.restype = None

lib.trapezoidal_method.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                                   ctypes.POINTER(ctypes.c_double), ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.trapezoidal_method.restype = None

# Function to run the solver for Euler's method
def solve_euler(x0, y0, v0, r, n, h):
    # Create C-style variables
    x = ctypes.c_double(x0)
    y = ctypes.c_double(y0)
    v = ctypes.c_double(v0)
    
    # Call the Euler method function
    lib.euler_method(ctypes.byref(x), ctypes.byref(y), ctypes.byref(v), r, n, h)
    
    # Return the results
    return x.value, y.value, v.value

# Function to run the solver for Trapezoidal method
def solve_trapezoidal(x0, y0, v0, r, n, h):
    # Create C-style variables
    x = ctypes.c_double(x0)
    y = ctypes.c_double(y0)
    v = ctypes.c_double(v0)
    
    # Call the Trapezoidal method function
    lib.trapezoidal_method(ctypes.byref(x), ctypes.byref(y), ctypes.byref(v), r, n, h)
    
    # Return the results
    return x.value, y.value, v.value

# Initialize parameters
x0, y0, v0 = 0.0, 1.0, 1.0  # Initial conditions
r = 0.1  # Some constant value (not used in this specific case, can be adjusted)
n = 1000  # Number of iterations
h = 0.01  # Step size

# Store data for plotting
x_vals = []
y_vals_euler = []
y_vals_trap = []

# Run the solvers
for i in range(n):
    x_euler, y_euler, v_euler = solve_euler(x0, y0, v0, r, i, h)
    x_trap, y_trap, v_trap = solve_trapezoidal(x0, y0, v0, r, i, h)
    
    x_vals.append(x_euler)
    y_vals_euler.append(y_euler)
    y_vals_trap.append(y_trap)

# Plot the results
plt.plot(x_vals, y_vals_euler, label='Sim2')
plt.plot(x_vals, y_vals_trap, label='Sim1', linestyle='dashed')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.title(' ')
plt.savefig("12.9.1.10.png")
plt.show()

