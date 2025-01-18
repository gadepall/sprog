import numpy as np

# Define the coefficients of the quadratic equation
a = 1
b = -40
c = 400

# Construct the companion matrix
A = np.array([[0, 1],
              [-c/a, -b/a]])

# Function for QR decomposition
def qr_decomposition(A):
    Q, R = np.linalg.qr(A)
    return Q, R

# Iterative QR decomposition to find the eigenvalues
def qr_iteration(A, iterations=100):
    for _ in range(iterations):
        Q, R = qr_decomposition(A)
        A = np.dot(R, Q)
    return np.diag(A)

# Run the QR iteration
eigenvalues = qr_iteration(A)
print("Eigenvalues of the companion matrix:", eigenvalues)

# Roots of the quadratic equation are the eigenvalues
roots = eigenvalues
print("Roots of the quadratic equation:", roots)

