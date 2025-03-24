import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file (code.so)
solver = ctypes.CDLL("./code.so")

# Define argument and return types for the C function
solver.fun.argtypes = (
    ctypes.c_double,  # x0
    ctypes.c_double,  # y0
    ctypes.c_double,  # h (step size)
    ctypes.POINTER(ctypes.c_double),  # x_points
    ctypes.POINTER(ctypes.c_double),  # y_points
    ctypes.c_int,  # num_points
)

# Number of points to compute
n = 100  # You can adjust this to match the C code output

# Create arrays to hold the results
x_points = (ctypes.c_double * n)()
y_points = (ctypes.c_double * n)()

# Call the function from the C code with initial conditions
solver.fun(0.0, 3.414 / 4, 0.1, x_points, y_points, n)

# Convert the results to NumPy arrays
x_points = np.array(x_points)
y_points = np.array(y_points)

# Define the second function: cos⁻¹(1/(√2 * cos(x)))
def second_function(x):
    return np.arccos(1 / (np.sqrt(2) * np.cos(x)))

# Generate data for the second function
x_vals = np.linspace(0.01, np.max(x_points), 1000)  # Avoid division by zero
y_vals = second_function(x_vals)

# Plot both results to compare
plt.figure(figsize=(10, 6))
plt.plot(x_points, y_points, label="sim", marker="o", linestyle='-', color='blue')
plt.plot(x_vals, y_vals, label="theory", linestyle="--", color='red')
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.xlim(0, 0.780)
plt.savefig("fig.png", dpi=300, bbox_inches="tight")
plt.show()

