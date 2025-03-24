import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./10.3.3.3.1.so')

# Define argument and return types for the functions
lib.createMat.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
lib.createMat.argtypes = [ctypes.c_int, ctypes.c_int]

lib.freeMat.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int]

lib.LU_decomp.restype = ctypes.POINTER(ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
lib.LU_decomp.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int]

lib.solveLU.restype = ctypes.c_int
lib.solveLU.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # L
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # U
    ctypes.POINTER(ctypes.c_double),  # b
    ctypes.POINTER(ctypes.c_double),  # x
    ctypes.c_int,  # dim
]

# Define dimensions and matrices
dim = 2
matrix = np.array([[1, -1], [1, -3]], dtype=np.float64)
b = np.array([26, 0], dtype=np.float64)

# Create matrix and vector in C
c_matrix = lib.createMat(dim, dim)
for i in range(dim):
    for j in range(dim):
        c_matrix[i][j] = matrix[i, j]

c_b = (ctypes.c_double * dim)(*b)
c_x = (ctypes.c_double * dim)()

# Perform LU decomposition
lu_result = lib.LU_decomp(c_matrix, dim)

# Extract L and U matrices
L = ctypes.cast(lu_result[0], ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
U = ctypes.cast(lu_result[1], ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))

# Solve Ax = b
status = lib.solveLU(L, U, c_b, c_x, dim)

# Check the status and process the result
if status == 0:
    print("No solution exists.")
elif status == -1:
    print("Infinite solutions exist.")
else:
    solution = [c_x[i] for i in range(dim)]
    print("Solution vector x:", solution)

# Plot the two lines
x_vals = np.linspace(35, 43, 500)  # Adjusted range to go up to 43
line1 = (1 * x_vals - 26) / 1  # y = x - 26
line2 = (1 * x_vals - 0) / 3  # y = x / 3

plt.figure(figsize=(8, 6))
plt.plot(x_vals, line1, label="x - y - 26 = 0", color='blue')
plt.plot(x_vals, line2, label="x - 3y = 0", color='red')

# Plot the solution point if unique
if status == 1:
    x, y = solution
    plt.plot(x, y, 'go', label=f'Solution: ({x:.2f}, {y:.2f})')  # Green dot for intersection

plt.axhline(35, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()
plt.xlabel('x')
plt.ylabel('y')
plt.title('Plot of the System of Equations')
plt.xlim(35, 43)  # Set x-axis range from 35 to 43
plt.ylim(min(min(line1), min(line2)), max(max(line1), max(line2)))  # Adjust y-limits for better view
plt.savefig('../figs/fig.png')
plt.show()

# Free allocated memory in C
lib.freeMat(c_matrix, dim)
lib.freeMat(L, dim)
lib.freeMat(U, dim)

