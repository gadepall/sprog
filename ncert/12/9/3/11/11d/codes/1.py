import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('/home/geediharsha/G2/1/codes/1.so')  # Ensure the shared library is in the correct path

# Define the function signature
lib.points.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_double)]

# Parameters
h = 0.01  # Step size
N = 100   # Number of steps

# Allocate memory for the result array (size N+1)
y = (ctypes.c_double * (N + 1))()

# Initialize the result array with zeros (just for safety)
for i in range(N + 1):
    y[i] = 0.0

# Call the C function
lib.points(h, y)

# Convert the result to a NumPy array
y_values = np.array(y[:])

# Generate x values
x_values = np.linspace(0, h * N, N + 1)

# Exact solution for comparison (Theory)
C1, C2 = 1.0, 0.0  # Example constants for C1e^x + C2e^-x
y_exact = C1 * np.exp(x_values) - C1 * np.exp(-x_values)

# Z-transform solution (Sim2)
z_transform_solution = np.array([1 + n for n in range(N + 1)])  # y[n] = (1 + n)u[n]

# Plot the results
plt.plot(x_values, y_values, label="Theory")
plt.plot(x_values, y_exact, label="Sim", linestyle="--")
plt.plot(x_values, z_transform_solution, label="Sim2", linestyle=":")  # Add Z-transform solution

# Configure plot labels and legend
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()

# Save the plot as an image file
plt.savefig("fig.png", dpi=300, bbox_inches='tight')  # Save as PNG with high resolution

# Show the plot
plt.show()

