import ctypes
import numpy as np

# Load the shared object library
lib = ctypes.CDLL('./code.so')

# Define argument and return types for the C function
lib.lu_decomposition.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
lib.lu_decomposition.restype = None

def lu_decomposition(A):
    n = len(A)
    A_flat = np.array(A, dtype=np.float64).flatten()  # Flatten A to pass to C
    L = np.zeros_like(A_flat)
    U = np.zeros_like(A_flat)

    # Call the C function
    lib.lu_decomposition(A_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                         L.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                         U.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                         n)
    
    # Reshape L and U back into matrices
    L = L.reshape(n, n)
    U = U.reshape(n, n)

    return L, U

# Example usage with a 3x3 matrix
A = [[1, -1], [2,3]]  # A 3x3 matrix
L, U = lu_decomposition(A)

print("Matrix L:")
print(L)

print("Matrix U:")
print(U)

