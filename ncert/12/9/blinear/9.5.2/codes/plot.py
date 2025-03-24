import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./libsolution.so')

# Define argument and return types for the C functions
lib.theoretical_solution.argtypes = [ctypes.c_double, ctypes.c_double]
lib.theoretical_solution.restype = ctypes.c_double

lib.numerical_solution.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                   np.ctypeslib.ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                                   np.ctypeslib.ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                                   ctypes.POINTER(ctypes.c_int)]
lib.numerical_solution.restype = None

# Define parameters
x_start = 1.0
x_end = 10.0
h = 0.1
C = 0.0  # Constant C in the theoretical solution

# Generate arrays for storing x and y values
n_points = int((x_end - x_start) / h) + 1
x_values = np.zeros(n_points)
y_values_theoretical = np.zeros(n_points)
y_values_numerical = np.zeros(n_points)

# Compute theoretical solution
for i in range(n_points):
    x_values[i] = x_start + i * h
    y_values_theoretical[i] = lib.theoretical_solution(x_values[i], C)

# Compute numerical solution
lib.numerical_solution(x_start, x_end, h, C, x_values, y_values_numerical, ctypes.byref(ctypes.c_int(n_points)))

# Plot both solutions
plt.plot(x_values, y_values_theoretical, label='Theoretical Solution', color='blue')
plt.plot(x_values, y_values_numerical, label='Numerical Solution (Improved Euler)', color='red', linestyle='--')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.title('Theoretical vs Numerical Solution')
plt.grid(True)
plt.show()
