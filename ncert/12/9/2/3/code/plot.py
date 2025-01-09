import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library (adjust path to the correct location)
lib = ctypes.CDLL('./func.so')  # Use './myfunctions.dll' on Windows

# Define argument and return types for the C functions
lib.solution.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_int]
lib.solution.restype = None

lib.derivative.argtypes = [ctypes.c_float, ctypes.c_float]
lib.derivative.restype = ctypes.c_float

# Set the initial values and parameters
x = ctypes.c_float(0.0)  # Initial value of x
y = ctypes.c_float(1.0)  # Initial value of y
n = 10000  # Number of iterations

# Create arrays to store the results for plotting
x_vals = []
y_vals = []
cos_vals = []

# Run the solution function and store results for plotting
for i in range(n):
    # Append current values to the lists
    x_vals.append(x.value)
    y_vals.append(y.value)
    
    # Calculate the exponential value of x (e^x)
    cos_vals.append(np.cos(x.value))
    
    # Call the C solution function
    lib.solution(ctypes.byref(x), ctypes.byref(y), 1)

# Plot both y(x) and exp(x)
sim_line, = plt.plot(x_vals, y_vals, label="sim", color='b')
theory_line, = plt.plot(x_vals, cos_vals, label="theory", color='r', linestyle='--')
plt.xlabel("x-axis")
plt.ylabel("y-axis")
# Customize axis spines for thick black axes
ax = plt.gca()  # Get the current axes
ax.spines['bottom'].set_position(('data', 0)) # Bottom axis
ax.spines['bottom'].set_linewidth(2)    # Set thickness
ax.spines['left'].set_position(('data', 0))   # Left axis
ax.spines['left'].set_linewidth(2)      # Set thickness
# Customize tick parameters for thicker black ticks
ax.tick_params(axis='both', colors='black', width=2, length=6)
plt.legend(handles=[sim_line, theory_line])
plt.grid(True)
plt.show()
