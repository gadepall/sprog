import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./gradient.so')

# Set argument and return types for the functions
gradient_lib.find_extremum.argtypes = [
    ctypes.c_double,        # x0
    ctypes.c_double,        # learning_rate
    ctypes.c_double,        # tol
    ctypes.POINTER(ctypes.c_double)  # extremum (output: [x_max, f_max, x_min, f_min])
]

# Gradient method to find extrema
def find_extremum(x0, learning_rate, tol):
    extremum = (ctypes.c_double * 4)()  # Array to hold [x_max, f_max, x_min, f_min]
    gradient_lib.find_extremum(x0, learning_rate, tol, extremum)
    return extremum[0], extremum[1], extremum[2], extremum[3]

# Parameters
x_start, x_end = 0, 2 * np.pi
learning_rate = 0.1
tol = 1e-6

# Find maximum and minimum
x_max, f_max, x_min, f_min = find_extremum(0.0, learning_rate, tol)

# Generate points for plotting
x_vals = np.linspace(x_start, x_end, 500)
y_vals = [np.sin(x) - np.cos(x) for x in x_vals]  # Directly compute the function in Python

# Plot the function and extrema
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label="f(x) = sin(x) - cos(x)", color="blue")
plt.scatter([x_max], [f_max], color="red", label=f"Maxima ")
plt.scatter([x_min], [f_min], color="green", label=f"Minima ")
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.grid()
plt.savefig('../figs/fig.png')
