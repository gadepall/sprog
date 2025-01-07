import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./c.so')

# Define the function prototype
lib.diffEqPoints.argtypes = [ctypes.c_int, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float]
lib.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

# Call the function to get the points (n, h, x, y1, y2, y3, y4)
n = 1000  # number of points
h = 0.01  # step size
x = 0.0   # initial x
y1 = 0.0  # initial y
y2 = 0.0  # initial y'
y3 = 0.0  # initial y''
y4 = 1.0  # initial y'''

# Call the function from the shared library
points_ptr = lib.diffEqPoints(n, h, x, y1, y2, y3, y4)

# Convert the result to a numpy array for easier manipulation
points = np.array([[points_ptr[i][0], points_ptr[i][1]] for i in range(n)])

# Free the memory in C after we're done
lib.freeMultiMem(points_ptr, n)

# Plot the results
plt.plot(points[:, 0], points[:, 1], label="sim")  # Add label for the legend
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)

# Display the legend
plt.legend()

plt.show()

