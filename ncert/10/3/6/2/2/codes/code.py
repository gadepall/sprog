import ctypes
import numpy as np
import matplotlib.pyplot as plt
import fractions

# Load the shared library
lib = ctypes.CDLL('./code.so')

# Define the argument and return types for the C function
N = 2  # Size of the matrix
lib.solve.argtypes = [np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
                      np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
                      np.ctypeslib.ndpointer(dtype=np.float32, flags="C_CONTIGUOUS")]

# Define the matrix and vector (Ax = b)
A = np.array([[2, 5],
              [3, 6]], dtype=np.float32)

b = np.array([1/4, 1/3], dtype=np.float32)

x = np.zeros(N, dtype=np.float32)

# Call the solve function from the C library
lib.solve(A, b, x)

# Print the solution
print("Solution x:", x)

# Generate x values
x_vals = np.linspace(-5, 5, 100)

# Calculate y values for both lines
y1_vals = ((1/4)-2*x_vals) / 5
y2_vals = ((1/3)-3*x_vals) / 6

# Plot the lines
plt.plot(x_vals, y1_vals, label="2p+5q = 1/4", color="red")
plt.plot(x_vals, y2_vals, label="3p+6q = 1/3", color="green")

# Convert intersection coordinates to fractions
x_frac = str(fractions.Fraction(float(x[0])).limit_denominator())
y_frac = str(fractions.Fraction(float(x[1])).limit_denominator())

# Plot the intersection point
plt.plot(x[0], x[1], 'ro', label=f'point of intersection')
plt.annotate(f'({x_frac}, {y_frac})', 
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

# Show the plot
plt.show()

