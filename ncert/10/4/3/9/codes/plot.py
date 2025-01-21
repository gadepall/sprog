import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library for QR decomposition and eigenvalue solver
libeigen = ctypes.CDLL('./libeigen.so')

# Define the argument and return types for the QRIterations function
libeigen.QRIterations.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Matrix A (flattened, real and imaginary parts interleaved)
    ctypes.c_int,  # maxIterations
    ctypes.c_double,  # tolerance
    ctypes.POINTER(ctypes.c_double)  # Eigenvalues (flattened 2x2 matrix)
]

# Prepare the 2x2 complex matrix
matrix = [
    [complex(0, 0), complex(93.75, 0)],
    [complex(1, 0), complex(8.75, 0)]
]

# Flatten the matrix and separate real and imaginary parts
N = len(matrix)
A_flat = np.zeros(2 * N * N, dtype=np.float64)

for i in range(N):
    for j in range(N):
        A_flat[2 * (i * N + j)] = matrix[i][j].real
        A_flat[2 * (i * N + j) + 1] = matrix[i][j].imag

# Prepare the array to hold eigenvalues
eigenvalues = np.zeros(4, dtype=np.float64)  # 2x2 matrix flattened

# Call the C function
libeigen.QRIterations(
    A_flat.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    500,  # Max iterations
    1e-20,  # Tolerance
    eigenvalues.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
)

# Print the eigenvalues
print("Eigenvalues:")
for i in range(N):
    re, im = eigenvalues[2 * i], eigenvalues[2 * i + 1]
    print(f"Eigenvalue {i+1}: {re:.6f} + {im:.6f}j")

# Load the shared library for the quadratic solver
solver = ctypes.CDLL('./quadratic_solver.so')

# Set up argument and return types for the C functions
solver.newton_raphson.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_int)]
solver.newton_raphson.restype = ctypes.c_double

solver.generate_points.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                   ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                                   ctypes.POINTER(ctypes.c_int)]

# Find the roots using Newton-Raphson
iterations1 = ctypes.c_int()
iterations2 = ctypes.c_int()
root1 = solver.newton_raphson(-100.0, ctypes.byref(iterations1))  # Initial guess far left
root2 = solver.newton_raphson(100.0, ctypes.byref(iterations2))   # Initial guess far right

# Generate points on the curve
x_min, x_max, step = -100, 100, 0.1
n_points = ctypes.c_int()
x_vals = (ctypes.c_double * 2000)()
y_vals = (ctypes.c_double * 2000)()

solver.generate_points(x_min, x_max, step, x_vals, y_vals, ctypes.byref(n_points))

x_vals_np = np.array(x_vals[:n_points.value])
y_vals_np = np.array(y_vals[:n_points.value])

# Plot the curve, y=0 line, and the intersection points
plt.figure(figsize=(12, 8))
plt.plot(x_vals_np, y_vals_np, label='y = 4x^2 - 35x - 375', linewidth=1.2)
plt.axhline(0, color='red', linestyle='--', label='y = 0')
plt.scatter([root1, root2], [0, 0], color='green', label=f'Roots: x1 = {root1:.6f}, x2 = {root2:.6f}')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()
plt.show()

# Print the roots and number of iterations
print(f"Root 1: {root1:.6f}")
print(f"Root 2: {root2:.6f}")

