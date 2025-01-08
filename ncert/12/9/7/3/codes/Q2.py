import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library (adjust path to the correct location)
lib = ctypes.CDLL('./verify.so')  # Use './myfunctions.dll' on Windows

# Define argument and return types for the C functions
lib.solution.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
lib.solution.restype = None

# Set the initial values and parameters
x = ctypes.c_double(0.001)  # Initial value of x
y = ctypes.c_double(0.031)  # Initial value of y
n = 19400  # Number of iterations

# Create arrays to store the results for plotting
x_vals = []
y_vals = []
exp_vals = []

# Run the solution function and store results for plotting
for i in range(n):
    # Append current values to the lists
    x_vals.append(x.value)
    y_vals.append(y.value)
    
    # Calculate the custom value for exp_vals
    exp_vals.append(math.sqrt((2.0 * x.value) - (x.value**2)) * ((math.sqrt(2))/2))
    
    # Call the C solution function
    lib.solution(ctypes.byref(x), ctypes.byref(y), 1)

# Plot both y(x) and exp(x)
sim_line, = plt.plot(x_vals, y_vals, label="sim", color='b')
theory_line, = plt.plot(x_vals, exp_vals, label="theory", color='r', linestyle='--')
plt.xlabel("x")
plt.ylabel("y and exp(x)")
plt.legend(handles=[sim_line, theory_line])
plt.grid(True)
plt.show()

