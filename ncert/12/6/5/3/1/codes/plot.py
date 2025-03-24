import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gen = ctypes.CDLL('./points.so')

# Define argument and return types for the functions
gen.generate_points.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
gen.generate_points.restype = None

FUNC_TYPE = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
gen.run_gradient_descent.argtypes = [
    ctypes.c_double, ctypes.c_double, FUNC_TYPE, 
    ctypes.c_double, ctypes.POINTER(ctypes.c_double)
]
gen.run_gradient_descent.restype = None

# Parameters
init_guess = 8.0
step_size = 0.01
min_value = ctypes.c_double(0.0)
num_points = 100
tolerance = 1e-5
# Create a numpy array for the points
points = np.zeros((num_points,2), dtype=np.double)

# Define the derivative function
def f_derivative(x):
    return 2 * x

# Call the generate_points function
gen.generate_points(
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Reshape the points array for easier plotting
points = points.reshape((num_points, 2))

# Call the run_gradient_descent function
gen.run_gradient_descent(
    init_guess, step_size, FUNC_TYPE(f_derivative), tolerance, ctypes.byref(min_value)
)

# Print the result
print(f"Minimum value: {min_value.value}")

# Plot the points and the minimum point
fig = plt.figure()
ax = plt.gca()
plt.plot(points[:, 0], points[:, 1], c='r', label='Objective function')
plt.scatter(min_value.value, min_value.value ** 2, c='b', label='Minimum Point')
ax.set_xlim(-10, 10)
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.legend(loc="best")

# Save the plot
plt.savefig('../figs/fig.png')

