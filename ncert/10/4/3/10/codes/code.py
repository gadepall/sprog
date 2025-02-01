import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Eigenvalues using libeigenvalues.so
lib_eigenvalues = ctypes.CDLL('./libeigenvalues.so')
lib_eigenvalues.find_eigenvalues.argtypes = [ctypes.c_int,
                                             np.ctypeslib.ndpointer(dtype=np.float64, ndim=2, flags="C_CONTIGUOUS"),
                                             np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags="C_CONTIGUOUS")]

# Define a matrix for eigenvalue calculation
matrix = np.array([[0, 1452],
                   [1, -11]], dtype=np.float64)

n = matrix.shape[0]
eigenvalues = np.zeros(n, dtype=np.float64)

# Call the eigenvalues function
lib_eigenvalues.find_eigenvalues(n, matrix, eigenvalues)

# Roots using librootfinder.so
lib_rootfinder = ctypes.CDLL('./librootfinder.so')
lib_rootfinder.find_roots_from_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib_rootfinder.find_roots_from_c.restype = None

# Create variables to store the roots
root1 = ctypes.c_double()
root2 = ctypes.c_double()

# Call the root-finding function
lib_rootfinder.find_roots_from_c(ctypes.byref(root1), ctypes.byref(root2))

# Check if the roots match the eigenvalues
match_root1 = np.isclose(eigenvalues[0], root1.value, atol=1e-6) or np.isclose(eigenvalues[1], root1.value, atol=1e-6)
match_root2 = np.isclose(eigenvalues[0], root2.value, atol=1e-6) or np.isclose(eigenvalues[1], root2.value, atol=1e-6)

# Plot both eigenvalues and roots on the same graph
a = -50  # Interval start
b = 50   # Interval end
p_values = np.linspace(a, b, 400)
f_values = p_values**2 + 11 * p_values - 1452

plt.figure(figsize=(10, 6))

# Plot the function
plt.plot(p_values, f_values, label='f(p) = p^2 + 11p - 1452', color='blue')

# Highlight eigenvalues
plt.scatter(eigenvalues, np.zeros_like(eigenvalues), color='red', label='Eigenvalues (from Matrix)', zorder=5)

# Highlight roots
plt.axvline(root1.value, color='green', linestyle='--', label=f'Root 1 (Newton\'s Method): {root1.value:.2f} {"[Match]" if match_root1 else ""}')
plt.axvline(root2.value, color='purple', linestyle='--', label=f'Root 2 (Newton\'s Method): {root2.value:.2f} {"[Match]" if match_root2 else ""}')

# Add grid and labels
plt.axhline(0, color='black', linewidth=0.5)
plt.title("Function Plot with Eigenvalues and Roots")
plt.xlabel('p')
plt.ylabel('f(p)')
plt.legend()
plt.grid(True)

# Save the figure
plt.tight_layout()
plt.savefig('eigenvalues_and_roots_highlighted.png')
plt.show()

