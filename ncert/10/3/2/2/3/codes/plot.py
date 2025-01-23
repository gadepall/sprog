import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./func.so')  # Compile the C code to a shared library (.so)

# Define the matrix structure in Python
class Matrix(ctypes.Structure):
    _fields_ = [("mat", ctypes.c_double * 4)]  # 2x2 matrix flattened to 1D (ORDER * ORDER)

# Define the C function prototypes
lib.luDecompose.argtypes = [Matrix, ctypes.POINTER(Matrix), ctypes.POINTER(Matrix)]
lib.luDecompose.restype = None

# Input matrix A (2x2)
A = Matrix((6, -3, 2, -1))

# Create empty matrices for L and U
L = Matrix((0,) * 4)
U = Matrix((0,) * 4)

# Call the C function for LU decomposition
lib.luDecompose(A, ctypes.byref(L), ctypes.byref(U))

# Convert the results back to Python-readable format
def matrix_to_numpy(mat):
    return np.array(mat.mat).reshape(2, 2)

L_np = matrix_to_numpy(L)
U_np = matrix_to_numpy(U)

# Print the results
print("Input Matrix A:")
print(matrix_to_numpy(A))
print("\nLower Triangular Matrix L:")
print(L_np)
print("\nUpper Triangular Matrix U:")
print(U_np)


# Plot the lines x - y = 8 and 3x - 3y = 16
x = np.linspace(-10, 10, 400)
y1 = (6/3)*x + (10/3)
y2 = 2*x + 9
plt.xlim(-10,10)
plt.ylim(-10,10)
plt.plot(x, y1, label='6x - 3y = -10')
plt.plot(x, y2, label='2x - y = -9')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.savefig("../figs/fig.png")
plt.show()

