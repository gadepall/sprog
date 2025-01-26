import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./line_and_lu.so')

# Define the functions from the shared library
lib.generate_points_line1.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                      ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                                      ctypes.POINTER(ctypes.c_int)]
lib.generate_points_line2.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                      ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                                      ctypes.POINTER(ctypes.c_int)]
lib.lu_decomposition.argtypes = [ctypes.c_double * 4, ctypes.c_double * 4, ctypes.c_double * 4]

# Function to generate points on line 1
def generate_points_line1(start, end, step):
    x = np.zeros(100, dtype=np.double)
    y = np.zeros(100, dtype=np.double)
    n = ctypes.c_int()
    lib.generate_points_line1(start, end, step, x.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                              y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), ctypes.byref(n))
    return x[:n.value], y[:n.value]

# Function to generate points on line 2
def generate_points_line2(start, end, step):
    x = np.zeros(100, dtype=np.double)
    y = np.zeros(100, dtype=np.double)
    n = ctypes.c_int()
    lib.generate_points_line2(start, end, step, x.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                              y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), ctypes.byref(n))
    return x[:n.value], y[:n.value]

# Function for LU decomposition
def lu_decomposition(matrix):
    flat_matrix = (ctypes.c_double * 4)(*matrix.flatten())
    L = (ctypes.c_double * 4)()
    U = (ctypes.c_double * 4)()
    lib.lu_decomposition(flat_matrix, L, U)
    return np.array(L).reshape(2, 2), np.array(U).reshape(2, 2)

# Generate points for both lines
x1, y1 = generate_points_line1(-10, 10, 0.5)
x2, y2 = generate_points_line2(-10, 10, 0.5)

# LU decomposition
matrix = np.array([[4/3, 2], [2, 3]])
L, U = lu_decomposition(matrix)
print("L matrix:")
print(L)
print("U matrix:")
print(U)

# Plot the lines
plt.plot(x1, y1, label="Line 1: 4/3x + 2y = 8", linewidth=3)
plt.plot(x2, y2, label="Line 2: 2x + 3y = 12")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.show()

