import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./libeigenvalues.so')

# Define the function signature
lib.find_eigenvalues.argtypes = [ctypes.c_int, 
                                 np.ctypeslib.ndpointer(dtype=np.float64, ndim=2, flags="C_CONTIGUOUS"),
                                 np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags="C_CONTIGUOUS")]

# Define a matrix 
matrix = np.array([[0, 1452],
                   [1, -11]], dtype=np.float64)

n = matrix.shape[0]
eigenvalues = np.zeros(n, dtype=np.float64)

# Call the C function
lib.find_eigenvalues(n, matrix, eigenvalues)

# Print the eigenvalues
print("Eigenvalues:", eigenvalues)

# Plot the eigenvalues
plt.scatter(eigenvalues, np.zeros_like(eigenvalues), color='red', label='Eigenvalues')
plt.axhline(0, color='black', linewidth=0.5)
plt.title('Eigenvalues of the Matrix')
plt.xlabel('Eigenvalue')
plt.ylabel('Magnitude')
plt.legend()
plt.grid(True)
plt.show()

