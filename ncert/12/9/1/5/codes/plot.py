import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file (assuming it's in the same directory)
code = ctypes.CDLL('./code.so')

# Define argument and return types for the solve function
code.solve.argtypes = [ctypes.POINTER(ctypes.c_double), 
                       ctypes.POINTER(ctypes.c_double), 
                       ctypes.POINTER(ctypes.c_double), 
                       ctypes.c_int]

# Parameters
h = 0.1  # step size
steps = 10000  # number of steps

# Initial values
x = 0.0
y = 0.0
v = 0.0

# Create arrays to hold the results
x_vals = np.zeros(steps)
y_vals = np.zeros(steps)
v_vals = np.zeros(steps)

# Prepare ctypes variables for passing to the solve function
x_ctypes = ctypes.c_double(x)
y_ctypes = ctypes.c_double(y)
v_ctypes = ctypes.c_double(v)

# Collect initial values
x_vals[0] = x
y_vals[0] = y
v_vals[0] = v

# Call the solve function and collect values at each step
for i in range(1, steps):
    # Call the solve function for each step
    code.solve(ctypes.byref(x_ctypes), ctypes.byref(y_ctypes), ctypes.byref(v_ctypes), 1)

    # Store the results after the update
    x_vals[i] = x_ctypes.value
    y_vals[i] = y_ctypes.value
    v_vals[i] = v_ctypes.value

# Plot the results
plt.plot(x_vals, y_vals, label='theory')
plt.xlabel('x')
plt.ylabel('y')
plt.scatter(x_vals, y_vals, color='red', s=4, label='sim')
plt.legend()
plt.show()

