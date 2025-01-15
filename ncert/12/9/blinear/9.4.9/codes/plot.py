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

# Function to compute sim-2 using the given difference equation
def compute_sim2(x_values, y0, h):
    y_values = [y0]
    for i in range(1, len(x_values)):
        xn = x_values[i]
        xn_1 = x_values[i - 1]
        yn = y_values[-1] + (h / 2) * (np.arcsin(xn) + np.arcsin(xn_1))
        y_values.append(yn)
    return np.array(y_values)

# Set parameters
x0 = 0
y0 = 1  
h = 0.01
n = 100

# Compute using C function
x_values, sim1_results = compute_trajectory(x0, y0, h, n)

# Compute sim-2 using the difference equation
sim2_results = compute_sim2(x_values, y0, h)

# Theoretical function: y = x*sin⁻¹(x) + sqrt(1 - x²)
y_theory = x_values * np.arcsin(x_values) + np.sqrt(1 - x_values**2)

# Plot results
plt.figure(figsize=(12, 8))
plt.scatter(x_values, sim1_results, color='blue', s=10, label='sim-1')
plt.plot(x_values, sim2_results, color='yellow', linewidth=5, label='sim-2')
plt.plot(x_values, y_theory, color='red', label='theory')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.show()

