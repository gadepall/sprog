import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library (the C code compiled into a shared object)
lib = ctypes.CDLL('./3.3.3.1.so')

# Set the argument and return types for the C functions
lib.solution.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # pointer to A
    ctypes.POINTER(ctypes.c_double),  # pointer to b
    ctypes.POINTER(ctypes.c_double)   # pointer to x
]
lib.solution.restype = None

# Define the size of the system (2x2 matrix)
N = 2

# Input matrix A and vector b
A = np.array([[1.0, -1.0], [1.0, -3.0]], dtype=np.float64)
b = np.array([26.0, 0.0], dtype=np.float64)
x = np.zeros(N, dtype=np.float64)  # Solution vector

# Call the C function to solve the system
lib.solution(A.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   b.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   x.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))

# Print the solution
print("Solution:")
print(f"x = {x[0]}")
print(f"y = {x[1]}")
# generating the points to plot lines.
x1 = np.linspace(30,45,15)
y1 = x1 - 26
y2 = x1/3
#plotting lines and solution.
plt.plot(x1,y1,color='r',label="line1")
plt.plot(x1,y2,color='g',label="line2")
plt.scatter(x[0],x[1],color='k',label = "solution")
plt.text(x[0],x[1],f'({x[0]},{x[1]})',ha = "right",va = "bottom")
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
plt.legend()
plt.show()

