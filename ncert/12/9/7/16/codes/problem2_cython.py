import numpy as np
import matplotlib.pyplot as plt
import ctypes
import os

# Load the C library
lib = ctypes.CDLL('./problem2_c_code.so')  # Ensure this path points to your compiled C library

# Define function signature for the C function
lib.compute_points.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # x_values array
    ctypes.POINTER(ctypes.c_double),  # y_values array
    ctypes.c_int,                     # num_points
    ctypes.c_double                   # h (step size)
]

# Parameters
num_points = 100
h = 0.01

# Arrays to store computed points (blue)
x_values = (ctypes.c_double * num_points)()
y_values = (ctypes.c_double * num_points)()

# Call the C function
lib.compute_points(x_values, y_values, num_points, h)

# Convert results to Python lists for plotting
x_values = np.array(list(x_values))
y_values = np.array(list(y_values))

# Exact function (green dashed curve)
x_exact = np.linspace(0, 1, 100)
y_exact = x_exact

# Plot the computed points (blue curve)
sim_line, = plt.plot(x_values, y_values, label="sim", color='blue')

# Plot the exact function (green dashed curve)
theory_line, = plt.plot(x_exact, y_exact, label="theory", color='chartreuse', linestyle='--')

# Customize axis spines for thick black axes
ax = plt.gca()
ax.spines['bottom'].set_color('black')
ax.spines['bottom'].set_linewidth(2)
ax.spines['left'].set_color('black')
ax.spines['left'].set_linewidth(2)

# Customize tick parameters for thicker black ticks
ax.tick_params(axis='both', colors='black', width=2, length=6)

# Finalize plot
plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.legend(handles=[sim_line, theory_line])
plt.grid(True)
plt.show()

