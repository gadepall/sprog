import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./liblu_decomposition.so')

# Define the argument and return types for the C function
N = 2  # Size of the matrix
lib.solve.argtypes = [np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
                      np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
                      np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS")]

# Define the matrix and vector (Ax = b)
A = np.array([[3, 2],
              [2, 3]], dtype=np.float32)

b = np.array([12, 13], dtype=np.float32)

x = np.zeros(N, dtype=np.float32)

# Call the solve function from the C library
lib.solve(A, b, x)

# Print the solution
print("Solution x:", x)

# Now we plot the lines and point of intersection
# Line 1: 4x - 2y = 3 -> y = (4x - 3)/2
# Line 2: x + 3y = 5 -> y = (5 - x)/3

# Generate x values
x_vals = np.linspace(-5, 5, 100)

# Calculate y values for both lines
y1_vals = (12-3*x_vals) / 2
y2_vals = (13-2*x_vals) / 3

# Plot the lines
plt.plot(x_vals, y1_vals, label="3u+2v = 12",color="red")
plt.plot(x_vals, y2_vals, label="2u+3v = 13",color="green")

# Plot the intersection point
plt.plot(x[0], x[1], 'ro', label=f'point of intersection')
plt.annotate(f'({x[0]:.2f}, {x[1]:.2f})', 
             xy=(x[0], x[1]), 
             xytext=(x[0]+0.5, x[1]+0.5),
             fontsize=12, color='black')
# Set plot labels
plt.xlabel('x')
plt.ylabel('y')
plt.axhline(0, color='black',linewidth=1)
plt.axvline(0, color='black',linewidth=1)
plt.grid(True)
plt.legend()

# Show the plot
plt.show()

