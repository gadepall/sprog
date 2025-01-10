import ctypes
import numpy as np
import matplotlib.pyplot as plt
# Load the shared library
lib = ctypes.CDLL('./difference_equation.so')

# Define the function signature
lib.compute_area_wrapper.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]
lib.compute_area_wrapper.restype = ctypes.c_double

# Set parameters
x_start = 1.0
x_end = 4.0
h = 0.001

# Call the function and print the result
area = lib.compute_area_wrapper(x_start, x_end, h)
print(area)


x = np.linspace(0, 5, 500)
plt.plot(x, 4 - 2 * x, label=r"$2x + y = 4$", color="blue")
plt.plot(x, (3 * x - 6) / 2, label=r"$3x - 2y = 6$", color="green")
plt.plot(x, (x + 5) / 3, label=r"$x - 3y + 5 = 0$", color="red")

# Find the points of intersection manually
A = (2, 0)  
B = (4, 3)  
C = (1, 2)  

# Highlight the intersection points
plt.scatter(*A, color="black")
plt.scatter(*B, color="black")
plt.scatter(*C, color="black")

# Shade the bounded area
x_fill = np.array([1, 2, 4])
y_fill = np.array([2, 0, 3])
plt.fill(x_fill, y_fill, color="gray", alpha=0.4, label="Bounded Area")

# Formatting the plot
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
plt.xlabel("x")
plt.ylabel("y")

plt.legend()
plt.grid()
plt.show()

