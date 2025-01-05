import os
import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
lib = ctypes.CDLL('./generate_points.so')

# Define the Points structure in Python
class Points(ctypes.Structure):
    _fields_ = [
        ('x', ctypes.POINTER(ctypes.c_double)),
        ('y', ctypes.POINTER(ctypes.c_double)),
        ('size', ctypes.c_int),
    ]

# Configure the function signature
lib.generate_points.restype = Points
lib.generate_points.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]

# Parameters
y0 = 1.0
h = 0.001
steps = 1000

# Generate points using the iterative method (Simulation)
points = lib.generate_points(ctypes.c_double(y0), ctypes.c_double(h), ctypes.c_int(steps))
x_sim = np.array([points.x[i] for i in range(points.size)])
y_sim = np.array([points.y[i] for i in range(points.size)])

# Generate theoretical points
x_theory = np.linspace(1.0, x_sim[-1], steps)  # Same range as simulation
y_theory = ((-x_theory**-4 + 2)**(-1/4)) # This equation is obtained solving it theoretically

# Create and plot the curves
plt.plot(x_sim, y_sim, label="Simulation", linestyle='-', color='blue')  # Simulation curve
plt.plot(x_theory, y_theory, label="Theory", linestyle='--', color='orange')  # Theory curve
plt.xlabel("x_n")
plt.ylabel("y_n")
plt.legend()
plt.grid(True)
plt.savefig('./figs/curve.png', dpi=300)



