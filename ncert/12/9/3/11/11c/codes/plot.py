import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
solver = ctypes.CDLL('./solver.so')

# Define the function signatures
solver.recorddata.restype = ctypes.POINTER(ctypes.c_double)
solver.recorddata.argtypes = [
    ctypes.c_double,  # lowerbound
    ctypes.c_double,  # upperbound
    ctypes.c_int,     # order
    ctypes.POINTER(ctypes.c_double),  # coefficients
    ctypes.POINTER(ctypes.c_double),  # initialconditions
    ctypes.c_double   # stepsize
]

# Define parameters
order = 2
lowerbound = 0.0
upperbound = 10.0
stepsize = 0.001
coefficients = np.array([1.0, 0.0, 0.0, 1.0], dtype=np.double) 
initialconditions = np.array([0.0, 0.0], dtype=np.double)

# Calculate the number of data points
no_datapoints = int((upperbound - lowerbound) / stepsize)

# Call the C function
results_ptr = solver.recorddata(
    ctypes.c_double(lowerbound),
    ctypes.c_double(upperbound),
    ctypes.c_int(order),
    coefficients.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    initialconditions.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    ctypes.c_double(stepsize)
)

# Convert results back to a NumPy array
results = np.ctypeslib.as_array(results_ptr, shape=(no_datapoints,))

# Generate x-values for plotting
x_values = np.arange(lowerbound + stepsize, upperbound + stepsize, stepsize)

# Calculate the y-values for the function y = -1/2 * x^2
y_function = -0.5 * x_values**2

# Plot the data
plt.scatter(x_values, results, color='blue', s=1, label='Sim.')
plt.plot(x_values, y_function, color='red', label='Theory')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.savefig('../figs/fig.png')
plt.show()

