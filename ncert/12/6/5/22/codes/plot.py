import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
lib = ctypes.CDLL('./points.so')

# Define the arguments and return types for the C functions
lib.Area.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
lib.Area.restype = ctypes.c_double

lib.generate_points.argtypes = [
    np.ctypeslib.ndpointer(dtype=np.float64),
    np.ctypeslib.ndpointer(dtype=np.float64),
    ctypes.c_int,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double
]

lib.gradient_descent.argtypes = [
    ctypes.c_double,
    ctypes.c_double,
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),
    ctypes.c_double,
    ctypes.POINTER(ctypes.c_double)
]

# Define the quadratic function in Python for gradient descent
def f(x):
    a = np.pi**2/4 + np.pi
    b = -7 * np.pi
    c = 49
    return 2 * a * x + b

# Convert Python function to C function pointer
C_FUNC_TYPE = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
f_c = C_FUNC_TYPE(f)

# Constants
a = np.pi**2 / 4 + np.pi
b = -7 * np.pi
c = 49
x_0 = 0
x_end = 14 / np.pi
n = 1000
h = 0.01
tolerance = 1e-6

# Allocate arrays for x and y
x = np.zeros(n, dtype=np.float64)
y = np.zeros(n, dtype=np.float64)

# Generate points using the C function
lib.generate_points(x, y, n, x_0, x_end, a, b, c)

# Find the minimum using gradient descent
minimum = ctypes.c_double()
lib.gradient_descent(x_0, h, f_c, tolerance, ctypes.byref(minimum))

print(f"Minimum point: x = {minimum.value}, y = {lib.Area(minimum.value, a, b, c)}")

# Plot the graph
plt.figure(figsize=(10, 6))
plt.plot(x, y, label="y = ax^2 + bx + c")
plt.scatter([minimum.value], [lib.Area(minimum.value, a, b, c)], color="red", label="Minimum Point", zorder=5)
#plt.title("Quadratic Function and its Minimum")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.savefig('../figs/fig.png')
plt.show()
