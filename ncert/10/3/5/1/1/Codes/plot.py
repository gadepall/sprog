import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lu_lib = ctypes.CDLL('./output.so')

# Define matrix and vector size
n = 2

# Define the new system of equations:
A = np.array([[1, -3], [3, -9]], dtype=np.float64)
b = np.array([3, 2], dtype=np.float64)

# Convert NumPy 2D array to `double**` for C functions
def to_c_matrix(matrix):
    row_pointers = (ctypes.POINTER(ctypes.c_double) * n)()
    for i in range(n):
        row_pointers[i] = (ctypes.c_double * n)(*matrix[i])
    return row_pointers

A_ctypes = to_c_matrix(A)
b_ctypes = (ctypes.c_double * n)(*b)

# Allocate memory for L, U, and solution vectors
L_ctypes = to_c_matrix(np.zeros((n, n), dtype=np.float64))
U_ctypes = to_c_matrix(np.zeros((n, n), dtype=np.float64))
x_doolittle = (ctypes.c_double * n)()
x_crout = (ctypes.c_double * n)()

# Define function prototypes
lu_lib.LU_Doolittle.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # A (2D array)
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # L
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # U
    ctypes.c_int
]

lu_lib.DoolittleSolver.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # L
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # U
    ctypes.POINTER(ctypes.c_double),  # b
    ctypes.POINTER(ctypes.c_double),  # x
    ctypes.c_int 
]

lu_lib.LU_Crouts.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # A (2D array)
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # L
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # U
    ctypes.c_int
]

lu_lib.CroutSolver.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # L
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # U
    ctypes.POINTER(ctypes.c_double),  # b
    ctypes.POINTER(ctypes.c_double),  # x
    ctypes.c_int 
]

# Solve using Doolittle's method
lu_lib.LU_Doolittle(A_ctypes, L_ctypes, U_ctypes, n)
lu_lib.DoolittleSolver(L_ctypes, U_ctypes, b_ctypes, x_doolittle, n)
solution_doolittle = [x_doolittle[i] for i in range(n)]

# Solve using Crout's method
lu_lib.LU_Crouts(A_ctypes, L_ctypes, U_ctypes, n)
lu_lib.CroutSolver(L_ctypes, U_ctypes, b_ctypes, x_crout, n)
solution_crout = [x_crout[i] for i in range(n)]

# Print the results
print(f"Solution using Doolittle's method: x = {solution_doolittle[0]:.4f}, y = {solution_doolittle[1]:.4f}")
print(f"Solution using Crout's method: x = {solution_crout[0]:.4f}, y = {solution_crout[1]:.4f}")

# Define the new equations to plot
x_vals = np.linspace(-5, 5, 100)
y1 = (3 - x_vals) / -3  # Equation 1: x - 3y = 3  => y = (3 - x) / -3
y2 = (2 - 3 * x_vals) / -9  # Equation 2: 3x - 9y = 2  => y = (2 - 3x) / -9

# Plot the equations
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y1, label="x - 3y = 3", color="blue")
plt.plot(x_vals, y2, label="3x - 9y = 2", color="red")

# Mark the intersection points
plt.scatter(solution_doolittle[0], solution_doolittle[1], color="black", marker="o", label=f"Doolittle ({solution_doolittle[0]:.2f}, {solution_doolittle[1]:.2f})")
plt.scatter(solution_crout[0], solution_crout[1], color="green", marker="x", label=f"Crout ({solution_crout[0]:.2f}, {solution_crout[1]:.2f})")

plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.axhline(0, color="black", linewidth=0.5)
plt.axvline(0, color="black", linewidth=0.5)
plt.grid(True, linestyle="--", linewidth=0.5)
plt.legend()
plt.show()
