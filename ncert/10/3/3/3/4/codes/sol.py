import numpy as np
from scipy.linalg import lu, solve

# Code to solve Ax = b by LU decomposition
A = np.array([[1, 10], [1, 15]], dtype=float)  # Modified A matrix
b = np.array([10, 4], dtype=float)  # Given b vector

# Fetch LU decomposition
P, L, U = lu(A)

# Solve Ly = b, Ux = y
y = solve(L, np.dot(P, b))  # Solve for y
x = solve(U, y)  # Solve for x

print("Solution vector x:", x)

