import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load shared object files
temp = ctypes.CDLL('./func.so')

# Define function signatures for func.so
temp.function.argtypes = [ctypes.c_double]
temp.function.restype = ctypes.c_double
temp.function2.argtypes = [ctypes.c_double]
temp.function2.restype = ctypes.c_double

temp.pointGen1.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
temp.pointGen1.restype = None
temp.pointGen2.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
temp.pointGen2.restype = None
temp.lu_decomposition.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # A
    ctypes.POINTER(ctypes.c_double),  # L
    ctypes.POINTER(ctypes.c_double),  # U
    ctypes.c_int                      # n
]

def lu_decomposition_py(A):
    n = A.shape[0]
    A_flat = A.flatten().astype(np.double)
    L = np.zeros((n, n), dtype=np.double)
    U = np.zeros((n, n), dtype=np.double)

    # Convert numpy arrays to C pointers
    A_ptr = A_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    L_ptr = L.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    U_ptr = U.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

    # Call the C function
    temp.lu_decomposition(A_ptr, L_ptr, U_ptr, n)

    return L, U

# Example usage
if __name__ == "__main__":
    A = np.array([[6, 3], [2, 4]], dtype=np.double)
    L, U = lu_decomposition_py(A)
    print("L:\n", L)
    print("U:\n", U)
    # Solving AX = B
    B = np.array([[6], [5]], dtype=np.double)
    Y = np.linalg.solve(L, B)  # Forward substitution
    X = np.linalg.solve(U, Y)  # Backward substitution
    print("Solution X:\n", X)


x0 = -2
num_points = 1000
x = (ctypes.c_double * num_points )()
y = (ctypes.c_double * num_points )()
x1 = (ctypes.c_double * num_points )()
y1 = (ctypes.c_double * num_points )()

temp.pointGen1(x0, x, y, num_points)
temp.pointGen2(x0, x1, y1, num_points)

x = np.array(x)
y = np.array(y)
x3=np.array([0.5])
y3=np.array([1])
plt.plot(x, y, color='red',linestyle='-', label='$6x+3y=6$')
plt.plot(x1, y1, color='blue',linestyle='-.', label ='$2x+4y=5$')
plt.plot(x3, y3, 'go', color='green')

plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.legend()
plt.grid()
plt.show()

