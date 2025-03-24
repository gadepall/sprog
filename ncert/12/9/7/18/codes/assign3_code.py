import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library created from the C code
c_lib = ctypes.CDLL('./curve_points.so')

# Define the function prototype
c_lib.generate_points.restype = ctypes.POINTER(ctypes.c_double)

# Parameters
h = 0.001
iterations = 1000

# Call the C function
points_ptr = c_lib.generate_points(ctypes.c_int(iterations), ctypes.c_double(h))

# Convert the result to a numpy array
points = np.ctypeslib.as_array(points_ptr, shape=(2 * iterations,))

# Extract x and y
x_sim = points[::2]
y_sim = points[1::2]

# Theory: y(x) = (-x^2 + 1) / e^x
x_theory = np.linspace(0, h * iterations, iterations)
y_theory = (-x_theory**2 + 1) / np.exp(x_theory)

# Plot both simulation and theory
plt.plot(x_sim, y_sim, label="Simulation", linestyle="--", linewidth=2, color="blue")
plt.plot(x_theory, y_theory, label="Theory", linewidth=1.5, color="orange")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.savefig('./figs/curve.png')

