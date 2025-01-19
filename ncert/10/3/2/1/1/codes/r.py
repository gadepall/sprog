import numpy as np
from scipy.linalg import lu, solve
#Code to solve Ax = b by LU decomposition
A = np.array([[1, 1], [-1,1]], dtype=float)
b = np.array([10,4], dtype=float)
#fecth LU decomposition
P, L, U = lu(A)
#solve Ly = b, Ux = y
y = solve(L, np.dot(P, b))
x = solve(U, y)
print("Solution vector x:", x)
