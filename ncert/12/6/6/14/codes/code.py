import matplotlib.pyplot as plt
import numpy as np
import ctypes

# Load the shared object file
temp = ctypes.CDLL('./func.so')

# Define argument and return types for C functions
temp.function.argtypes = [ctypes.c_double]
temp.function.restype = ctypes.c_double

temp.derivative.argtypes = [ctypes.c_double]
temp.derivative.restype = ctypes.c_double

temp.pointGen.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
temp.gradient_descent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.POINTER(ctypes.c_double), ctypes.c_int]

# Parameters
start = 0.0
end = np.pi
h = 0.001

x0 = np.pi / 4  # Initial guess
alpha = 0.01

# Correct number of points
num_points = int((end - start) / h) + 1

# Allocate memory for x and y arrays
x = (ctypes.c_double * num_points)()
y = (ctypes.c_double * num_points)()

# Allocate memory for extrema results
absmax = (ctypes.c_double * 2)()
absmin = (ctypes.c_double * 2)()

# Generate points for the function plot
temp.pointGen(start, end, x, y)

# Perform Gradient Ascent to find the maximum
temp.gradient_descent(x0, alpha, absmax, 1)

# Perform Gradient Descent to find the minimum
temp.gradient_descent(x0, alpha, absmin, 0)

# Convert C arrays to NumPy arrays for plotting
x = np.array(x)
y = np.array(y)

# Print the results
print(f"Maximum at (x = {absmax[0]}, f(x) = {absmax[1]})")
print(f"Minimum at (x = {absmin[0]}, f(x) = {absmin[1]})")

# Plot the function and the found extremum points
plt.plot(x, y, color='blue', label=r'$f(x) = \cos^2(x) + \sin(x)$')
plt.scatter(absmax[0], absmax[1], color='green', label='Maximum', zorder=5)
plt.scatter(absmin[0], absmin[1], color='red', label='Minimum', zorder=5)
plt.xlabel('X-AXIS')
plt.ylabel('Y-AXIS')
plt.title(r'Plot of $f(x) = \cos^2(x) + \sin(x)$')
plt.grid()
plt.legend()
plt.savefig('../figure/fig.png')
plt.show()

