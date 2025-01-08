import ctypes
import matplotlib.pyplot as plt
import numpy as np

# Provide the full path to the shared object if it's not in the same directory
lib = ctypes.CDLL('/home/suraj5323/EE1003/9.6.18/codes/code.so')

# Define the argument types and return type for the points function
lib.points.argtypes = [
    ctypes.c_float,  # x0: Initial x value
    ctypes.c_float,  # y0: Initial y value
    ctypes.c_float,  # h: Step size
    ctypes.c_int,    # n: Number of iterations
    ctypes.POINTER(ctypes.c_float),  # x_points: Output x array
    ctypes.POINTER(ctypes.c_float)   # y_points: Output y array
]
lib.points.restype = None

# Input parameters
x0 = 1  # Initial value of x
y0 = 1  # Initial value of y
h = 0.01  # Step size
n = 100   # Number of iterations

# Allocate memory for the result arrays
x_points = (ctypes.c_float * n)()
y_points = (ctypes.c_float * n)()

# Call the `points` function from the shared library
lib.points(x0, y0, h, n, x_points, y_points)

# Convert the ctypes arrays to Python lists
x_values = [x_points[i] for i in range(n)]
y_values = [y_points[i] for i in range(n)]

# Plot the simulation results
plt.plot(x_values, y_values, label="sim", marker='o', markersize=3, color='blue')

# Theoretical function: y = math.radians(180)/2 - x
x0 = 1
y0 = 1
h = 0.01
n = 100
x_point = [x0]
y_point = [y0]

for i in range(n):
    x_n = x_point[-1] + h
    y_n = 2*x_point[-1]*x_point[-1]- x_point[-1] #  formula
    x_point.append(x_n)
    y_point.append(y_n)

# Plot the correct graph
plt.plot(x_point, y_point, label="theory",color='black')

# Labels for the second graph
plt.xlabel("x")
plt.ylabel("y")
plt.legend()

plt.savefig("Figure_1.png")
