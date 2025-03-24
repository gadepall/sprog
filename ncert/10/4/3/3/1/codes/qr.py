#Code by S.Sai AKshita
#importing numpy library
import numpy as np


#defining function which performs QR algorithm
def qr_algorithm(C, max_iter=1000, tol=1e-9):

#    Perform QR decomposition to approximate the eigenvalues of a matrix.
    
#    Parameters:
#        C (numpy.ndarray): The companion matrix.
 #       max_iter (int): Maximum number of iterations.
  #      tol (float): Convergence tolerance for off-diagonal elements.
    
   # Returns:
   #     numpy.ndarray: Approximate eigenvalues of the matrix.
    A = C.copy()
    for i in range(max_iter):
        # QR decomposition
        Q, R = np.linalg.qr(A)
        # Update the matrix
        A = R @ Q
        # Check for convergence (off-diagonal elements close to zero)
        if np.allclose(A - np.diag(np.diagonal(A)), 0, atol=tol):
            break
    return np.diagonal(A)

# Define the companion matrix for x^2 - 3x - 1 = 0
C = np.array([
    [0, 1],
    [1, 3]
])

# Perform QR algorithm
eigenvalues = qr_algorithm(C)

# Print the results
print("Eigenvalues of the companion matrix:")
print(eigenvalues)


