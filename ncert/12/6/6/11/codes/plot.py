import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the .so file
code = ctypes.CDLL('./code.so')

# Set argument and return types for the functions
code.fun.argtypes = [
    ctypes.c_double,  # Initial b
    ctypes.c_double,  # Learning rate
    ctypes.POINTER(ctypes.c_double),  # b_values array
    ctypes.POINTER(ctypes.c_double),  # A_values array
]

# Parameters
b = 0.0  # Initial value of b
alpha = 0.01  # Learning rate
max_iter = 100  # Maximum number of iterations

# Allocate arrays for b_values and A_values
b_values = (ctypes.c_double * max_iter)()
A_values = (ctypes.c_double * max_iter)()

# Call the `fun` function
code.fun(ctypes.c_double(b), ctypes.c_double(alpha), b_values, A_values)

# Convert results to NumPy arrays
b_values_np = np.array(b_values[:max_iter])
A_values_np = np.array(A_values[:max_iter])

# Include (0, 0) in the plot by removing the filtering for valid indices
b_values_np = np.insert(b_values_np, 0, 0)  # Insert 0 at the beginning
A_values_np = np.insert(A_values_np, 0, 0)  # Insert 0 at the beginning

# Plot Area vs. b
plt.figure(figsize=(8, 6))

# Plot the area as a function of b
plt.plot(b_values_np, A_values_np, marker='o', color='orange', label='Area vs. b')
plt.xlabel('b (width)')
plt.ylabel('Area (A)')
plt.grid()
plt.legend()

# Save the plot as a PNG file
plt.tight_layout()
plt.savefig('plot.png')

# Optionally, display the plot
# plt.show()
