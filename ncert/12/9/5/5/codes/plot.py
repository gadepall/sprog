
import ctypes
import sympy as sp
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library (adjust path to the correct location)
lib = ctypes.CDLL('./main.so')  # Use './myfunctions.dll' on Windows

# Define argument and return types for the C functions
lib.solution.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_int]
lib.solution.restype = None

lib.derivative.argtypes = [ctypes.c_float, ctypes.c_float]
lib.derivative.restype = ctypes.c_float

# Set the initial values and parameters
x = ctypes.c_float(1)  # Initial value of x
y = ctypes.c_float(0)  # Initial value of y
n = 100000  # Number of iterations

# Create arrays to store the results for plotting
x_vals = []
y_vals = []
exp_vals = []

# Run the solution function and store results for plotting
for i in range(n):
    # Append current values to the lists
    x_vals.append(x.value)
    y_vals.append(y.value)
    
    # Calculate the exponential value of x (e^x)
    exp_vals.append(-(x * (np.sqrt(2) / 2 * (-np.abs(x)**(2 * np.sqrt(2)) + 1)) / (1 + np.abs(x)**(2 * np.sqrt(2)))))
    
    # Call the C solution function
    lib.solution(ctypes.byref(x), ctypes.byref(y), 1)

# Plot both y(x) and exp(x)
sim_line, = plt.plot(x_vals, y_vals, label="sim", color='b')
theory_line, = plt.plot(x_vals, exp_vals, label="theory", color='r', linestyle='--')
plt.xlabel("x-axis")
plt.ylabel("y-axis")
# Customize axis spines for thick black axes
ax = plt.gca()  # Get the current axes
ax.spines['bottom'].set_color('black')  # Bottom axis
ax.spines['bottom'].set_linewidth(2)    # Set thickness
ax.spines['left'].set_color('black')    # Left axis
ax.spines['left'].set_linewidth(2)      # Set thickness
# Customize tick parameters for thicker black ticks
ax.tick_params(axis='both', colors='black', width=2, length=6)
plt.legend(handles=[sim_line, theory_line])
plt.grid(True)
plt.show()
