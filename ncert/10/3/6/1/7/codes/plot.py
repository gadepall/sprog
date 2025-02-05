import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./a3.so')

# Define the argument and return types for the C function
N = 2  # Size of the matrix
lib.solve.argtypes = [np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
                      np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
                      np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS")]

# Define the matrix and vector (Ax = b)
A = np.array([[10, 2],
              [15, -5]], dtype=np.float32)

b = np.array([4, -2], dtype=np.float32)

x = np.zeros(N, dtype=np.float32)

# Call the solve function from the C library
lib.solve(A, b, x)

# Now we plot the lines and point of intersection
# Line 1: 10u + 2v = 4 -> y = (4 - 10x) / 2
# Line 2: 15u - 5v = -2 -> y = (15x + 2) / 5

# Generate x values
x_vals = np.linspace(-5, 5, 100)

# Calculate y values for both lines
y1_vals = (4 - 10*x_vals) / 2
y2_vals = (15*x_vals + 2) / 5

# Plot the lines
plt.plot(x_vals, y1_vals, label="10u+2v = 4", color="red")
plt.plot(x_vals, y2_vals, label="15u-5v = -2", color="green")

# Plot the intersection point
plt.plot(x[0], x[1], 'bo', label=f'point of intersection')
plt.annotate(f'({x[0]:.2f}, {x[1]:.2f})', 
             xy=(x[0], x[1]), 
             xytext=(x[0]+0.5, x[1]+0.5),
             fontsize=12, color='black')

# Set plot labels
plt.xlabel('x')
plt.ylabel('y')
plt.axhline(0, color='black', linewidth=1)
plt.axvline(0, color='black', linewidth=1)
plt.grid(True)
plt.legend()

# save the plot
plt.savefig('../figs/fig.png')

