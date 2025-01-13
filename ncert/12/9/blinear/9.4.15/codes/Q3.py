import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
methods = ctypes.CDLL('./methods.so')

# Parameters
x_start = 0.0
x_end = 4.0
h = 0.01
y0 = 0.0
n = int((x_end - x_start) / h) + 1

# Allocate memory for x and y arrays
x = np.zeros(n, dtype=np.float64)
y_trap = np.zeros(n, dtype=np.float64)
y_laplace = np.zeros(n, dtype=np.float64)

# Set up argument and return types for Trapezoidal Method
methods.trapezoidal_method.argtypes = [
    ctypes.c_double,  # x_start
    ctypes.c_double,  # x_end
    ctypes.c_double,  # h
    ctypes.c_double,  # y0
    ctypes.c_int,     # n
    np.ctypeslib.ndpointer(dtype=np.float64),  # x array
    np.ctypeslib.ndpointer(dtype=np.float64),  # y array
]
methods.trapezoidal_method.restype = None

# Set up argument and return types for Laplace Method
methods.laplace_method.argtypes = [
    ctypes.c_double,  # x_start
    ctypes.c_double,  # x_end
    ctypes.c_double,  # h
    ctypes.c_int,     # n
    np.ctypeslib.ndpointer(dtype=np.float64),  # x array
    np.ctypeslib.ndpointer(dtype=np.float64),  # y array
]
methods.laplace_method.restype = None

# Call the C functions
methods.trapezoidal_method(x_start, x_end, h, y0, n, x, y_trap)
methods.laplace_method(x_start, x_end, h, n, x, y_laplace)

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(x, y_trap, label="Trapezoidal Method", linestyle="--", color="blue")
plt.plot(x, y_laplace, label="Laplace Method (Analytical)", color="red")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.show()

