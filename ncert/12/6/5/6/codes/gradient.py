import ctypes
import numpy as np
import matplotlib.pyplot as plt

#Derivative of function 
def function_prime(x):
    return -72 - 36 * x

# Original function =
def function(x):
    return 41 - 72 * x - 18 * x ** 2

# Load the shared C library
lib = ctypes.CDLL('./gradient.so')

# Define the argument and return types for the C functions
lib.gradient_ascent.restype = ctypes.c_double
lib.gradient_ascent.argtypes = [
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),  # Function pointer for derivative
    ctypes.c_double,  # Initial guess
    ctypes.c_double,  # Learning rate
    ctypes.c_double,  # Tolerance
    ctypes.c_int       # Maximum iterations
]

# Convert the Python function to a C-compatible function pointer
C_FUNC_TYPE = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
c_function_prime = C_FUNC_TYPE(function_prime)

# Gradient ascent parameters
initial_guess = 0.0
learning_rate = 0.01
tolerance = 1e-5
max_iter = 10000

# Call the C function
converged_x = lib.gradient_ascent(c_function_prime, initial_guess, learning_rate, tolerance, max_iter)
print(converged_x)
# Plot the function and the maximum point
x_vals = np.linspace(-6, 4, 100)
y_vals = function(x_vals)
y_max = function(converged_x)

plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label='Objective Function', color='blue')
plt.scatter([converged_x], [y_max], color='red', label=f'Maximum Point')
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.savefig('../figs/fig.png')
plt.show()

