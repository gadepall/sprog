import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
solve = ctypes.CDLL('./code.so')

# Define the function signature
solve.solve.argtypes = [
    ctypes.c_double,  # Step size (h)
    ctypes.POINTER(ctypes.c_double),  # Array y
    ctypes.POINTER(ctypes.c_double),  # Array dy
    ctypes.c_int       # Number of steps
]

# Initialize parameters
steps = 100
h = 0.1
y = np.zeros(steps + 1, dtype=np.float64)  # Array for y
dy = np.zeros(steps + 1, dtype=np.float64)  # Array for dy

# Call the C function
solve.solve(
    ctypes.c_double(h),
    y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    dy.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    ctypes.c_int(steps)
)

# Generate x values
x = np.linspace(0, h * steps, steps + 1)

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(x, x, label="y = x", color='r', linestyle="--")  # Reference line
plt.plot(x, y, label="y(x) (from C)", color='b', linestyle="-")
plt.xlabel("x")
plt.ylabel("Values")
plt.xlim(0,10)
plt.ylim(0,10)
#plt.title("Comparison: y(x) and y = x")
plt.legend()
plt.grid()
plt.savefig("../figure/fig.png")
plt.show()
