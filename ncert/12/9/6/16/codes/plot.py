import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file (ensure 'code.so' is in the same directory)
code = ctypes.CDLL('./code.so')

# Define argument types for the `solve` function
code.solve.argtypes = [ctypes.POINTER(ctypes.c_double), 
                       ctypes.POINTER(ctypes.c_double), 
                       ctypes.c_int]

# Parameters
h = 0.001  # Step size 
steps = 10000  # Number of steps for the Euler's method

# Initial values
x = 0.0  # Initial value of x
y = 1.0  # Initial value of y (e.g., solving dy/dx = x + y with y(0) = 1)

# Create arrays to hold results
x_vals = np.zeros(steps)
y_vals = np.zeros(steps)

# Prepare ctypes variables for passing to the `solve` function
x_ctypes = ctypes.c_double(x)
y_ctypes = ctypes.c_double(y)

# Store initial values
x_vals[0] = x
y_vals[0] = y

# Call the solve function in a loop to calculate values at each step
for i in range(1, steps):
    code.solve(ctypes.byref(x_ctypes), ctypes.byref(y_ctypes), 1)  # Call the C function for 1 step
    x_vals[i] = x_ctypes.value  # Store updated x value
    y_vals[i] = y_ctypes.value  # Store updated y value

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(x_vals, y_vals, label="Theory", color="blue")
plt.scatter(x_vals, y_vals, color='red', s=4, label='Sim')
plt.xlabel('x')
plt.ylabel('y')
plt.grid()
plt.legend()
plt.show()

