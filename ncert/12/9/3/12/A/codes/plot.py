import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object (compiled C code)
lib = ctypes.CDLL('./libmodel.so')

# Define the argument types for the C function
lib.value.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib.value.restype = None  # The function has no return value

# Initialize the step size (h), and arrays for y and dy
h = 0.1  # Step size
y = np.zeros(52, dtype=np.float64)  # Array for y(x), initialize with zeros
dy = np.ones(52, dtype=np.float64)  # Array for y'(x), initialize with ones (initial condition)

# Call the C function to compute the values of y(x) and y'(x)
lib.value(h, y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), dy.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))

# Generate the x values from 0 to 5 with the step size h
x = np.arange(0, h * 52, h)
xx = np.linspace(0, 5, 50)
yy = xx**2
# Plot the graph of x vs y(x)
plt.plot(x, y, label="y(x)", color='b')

# Plot the graph of y = x for comparison
plt.plot(xx, xx, label="y = x", color='r', linestyle='--')  

# Label the points on the y(x) graph
#for i in range(0, len(x), 5):  # Label every 5th point
 #   plt.text(x[i], y[i], f'({x[i]:.2f}, {y[i]:.2f})', fontsize=8, color='black', ha='right')

# Set the title and labels for the axes
#plt.title("Plot of x vs y(x) with y = x comparison")
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(0,5)
plt.ylim(0,10)

# Show the plot
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")
plt.show()


