import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the compiled C shared library
lib = ctypes.CDLL('./solver.so')

# Define the argument and return types for the C functions
lib.recorddata_trapezoid.restype = ctypes.POINTER(ctypes.c_double)
lib.recorddata_trapezoid.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]

lib.recorddata_ztransform.restype = ctypes.POINTER(ctypes.c_double)
lib.recorddata_ztransform.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]

# Define parameters
lowerbound = 0.0
upperbound = 10.0
stepsize = 0.1
initialy0 = 1.0
initialyprime0 = 0.0
no_datapoints = int((upperbound - lowerbound) / stepsize) + 1

# Call the C functions
results_trapezoid = lib.recorddata_trapezoid(lowerbound, upperbound, stepsize, initialy0, initialyprime0)
results_ztransform = lib.recorddata_ztransform(lowerbound, upperbound, stepsize, initialy0, initialyprime0)

# Convert results to Python lists
sim1 = [results_trapezoid[i] for i in range(no_datapoints)]
sim2 = [results_ztransform[i] for i in range(no_datapoints)]

# Free the allocated memory in C
lib.free(results_trapezoid)
lib.free(results_ztransform)

# Generate theoretical values
def theory_function(x):
    return 0.5 * (np.exp(-x) + np.exp(x))

theory = [theory_function(i * stepsize) for i in range(no_datapoints)]

# Generate x values
x_values = [i * stepsize for i in range(no_datapoints)]

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(x_values, sim1, label='Sim1', linestyle='--', color = 'black')
plt.plot(x_values, sim2, label='Sim2', linestyle='--', color = 'green')
plt.plot(x_values, theory, label='Theory', color = 'red')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Comparison of Simulations and Theoretical Function')
plt.legend()
plt.grid()
plt.savefig("../figs/fig.png")
plt.show()

