import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_double

# Load the shared library
lib = CDLL('./code.so')

# Define the function prototype
lib.fun.argtypes = [c_double, c_double, c_double, np.ctypeslib.ndpointer(dtype=np.float64, flags='C_CONTIGUOUS'), np.ctypeslib.ndpointer(dtype=np.float64, flags='C_CONTIGUOUS')]
lib.fun.restype = None

# Allocate memory for the x and y values for the first simulation
x_vals = np.zeros(100, dtype=np.float64)
y_vals = np.zeros(100, dtype=np.float64)

# Initial values for the first simulation
x = 0.0
y = 1.0
h = 0.1

# Call the C function for the first simulation
lib.fun(x, y, h, x_vals, y_vals)

# Allocate memory for the x and y values for the second simulation
x_vals2 = np.zeros(100, dtype=np.float64)
y_vals2 = np.zeros(100, dtype=np.float64)

# Initial values for the second simulation (e.g., slightly different step size or initial conditions)
y2 = 1.0  # Same y initial value
h2 = 0.05  # Smaller step size for Sim 2

# Call the C function for the second simulation
lib.fun(x, y2, h2, x_vals2, y_vals2)

# Plot the first simulation results (dots in blue)
plt.plot(x_vals, y_vals, 'bo', label="Sim 1")

# Plot the second simulation results (thick yellow broken lines)
plt.plot(x_vals2, y_vals2, 'y--', linewidth=5, label="Sim 2")

# Plot cos(x) for comparison (black thick line)
x_vals_cos = np.linspace(0, 10, 100)
plt.plot(x_vals_cos, np.cos(x_vals_cos), 'k-', linewidth=2, label="Theory")

# Add labels, legend, and save the figure as 'plot.png'
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.savefig('plot.png')  # Save the plot as 'plot.png'
plt.show()

