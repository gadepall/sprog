import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_int, c_double, POINTER

# Load the compiled C library
lib = CDLL('./compute.so')

# Specify the argument and return types for the C function
lib.compute.argtypes = [c_int, c_double, POINTER(c_double), POINTER(c_double)]
lib.compute.restype = None

# Function to compute using C code
def compute_trajectory(x0, y0, h, n):
    # Allocate memory for results
    x = (c_double * (n + 1))()
    y = (c_double * (n + 1))()
    
    # Set initial values
    x[0] = x0
    y[0] = y0

    # Call the C function
    lib.compute(n, h, x, y)

    # Convert results to NumPy arrays
    return np.array(x[:n+1]), np.array(y[:n+1])

# Set parameters
x0 = 0
y0 = 1
h = 0.01
n = 500

# Compute using C function
x_values, results = compute_trajectory(x0, y0, h, n)

# Theoretical function
y_function = np.sqrt(1 + x_values**2)

# Plot results
plt.figure(figsize=(10, 6))
plt.scatter(x_values, results, color='blue', s=1, label='sim')
plt.plot(x_values, y_function, color='red', label='theory')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.title('Comparison of Simulated and Theoretical Results')
plt.grid(True)  
plt.show()

