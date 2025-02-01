import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared library (make sure the path is correct)
lib = ctypes.CDLL('./libgradient_descent.so')

# Define argument types and return types for the gradient_descent function
lib.gradient_descent.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib.gradient_descent.restype = None

# Function definition in Python
def f(x):
    return x**3 - 3*x + 3

# Set up the range of x values for plotting
x_values = np.linspace(-2, 2, 400)
y_values = f(x_values)

# Critical points (local min at x=1, local max at x=-1) based on the C code
local_min_x = ctypes.c_double()
local_min_y = ctypes.c_double()

local_max_x = ctypes.c_double()
local_max_y = ctypes.c_double()

# Call gradient descent to find the local minimum
lib.gradient_descent(ctypes.c_double(1), ctypes.byref(local_min_x), ctypes.byref(local_min_y))
lib.gradient_descent(ctypes.c_double(-1), ctypes.byref(local_max_x), ctypes.byref(local_max_y))

# Print the local minimum and maximum points
print(f"Local minimum found at x = {local_min_x.value}")
print(f"Local maximum found at x = {local_max_x.value}")

# Plot the function and the critical points
plt.plot(x_values, y_values, label="f(x) = x^3 - 3x + 3", color='green')

# Highlight the local minima and maxima
plt.scatter(local_min_x.value, local_min_y.value, color='blue', label="Local Min (x=1)")
plt.scatter(local_max_x.value, local_max_y.value, color='red', label="Local Max (x=-1)")

# Add labels and title
plt.xlabel("x")
plt.ylabel("f(x)")
#plt.title("Plot of f(x) = x^3 - 3x + 3 with Local Minima and Maxima")
plt.axhline(0, color='black',linewidth=1)
plt.axvline(0, color='black',linewidth=1)
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")
plt.show()

