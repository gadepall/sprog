import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the C shared library
generatepts = ctypes.CDLL('./generatepts.so')

# Define the function signatures for the C functions
generatepts.matrix_method.restype = ctypes.POINTER(ctypes.c_double)  # Return type is a pointer to double
generatepts.matrix_method.argtypes = [
    ctypes.c_int,                 # N: Number of time steps
    ctypes.c_double,              # h: Step size
    ctypes.POINTER(ctypes.c_double),  # y0: Initial conditions array
    ctypes.POINTER(ctypes.c_double),  # coefficients: Coefficients array
    ctypes.c_int                  # order: Order of the system
]

# Parameters for the system
order = 3
lower_bound = 0.0
upper_bound = 100.0
h = 0.01
initial_conditions = np.array([1.0, -1.0, 1.0], dtype=np.double)  # Initial conditions in the form [y(0),y'(0),y''(0)]
coefficients = np.array([0.0, 1.0, 2.0, 1.0], dtype=np.double)       # Coefficients of the differential equation in form [a0,a1,a2,a3,...] ai specifies coeffiecent of yi

# Calculate the number of data points (steps)
N = int((upper_bound - lower_bound) / h)

# Call the C function to solve the system using Euler's method
results_ptr = generatepts.matrix_method(
    ctypes.c_int(N),
    ctypes.c_double(h),
    initial_conditions.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    coefficients.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    ctypes.c_int(order)
)

# Convert the C pointer result to a NumPy array
results = np.ctypeslib.as_array(results_ptr, shape=(N + 1,))

# Generate x-values for plotting
x_values = np.linspace(lower_bound, upper_bound, N + 1)

# Theoretical function for comparison (e.g., e^(-x))
y_function = np.exp(-x_values)

# Plot the data
plt.scatter(x_values, results, color='blue', s=1, label='Simulated (Euler)')
plt.plot(x_values, y_function, color='red', label="Theory ($e^{-t}$)")
plt.xlabel('Time (t)')
plt.ylabel('y(t)')
plt.legend()
plt.grid(True)
plt.savefig('../figs/fig.png')  # Save the figure
plt.show()

