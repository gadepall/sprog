import numpy as np
from scipy.linalg import lu, solve
#Code to solve Ax = b by LU decomposition
A = np.array([[1, -7], [1, -3]], dtype=float)
b = np.array([-42, 6], dtype=float)
#fecth LU decomposition
P, L, U = lu(A)
#solve Ly = b, Ux = y
y = solve(L, np.dot(P, b))
x = solve(U, y)
print("Solution vector x:", x)

