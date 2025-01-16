import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
newton_solver = ctypes.CDLL('./c.so')

# Set the return type for the C function
newton_solver.newton_method.restype = ctypes.c_double

# Call the Newton's method function in C
initial_guess = 8.0  # Starting guess for the root
tolerance = 1e-6      # Convergence tolerance
max_iter = 1000       # Maximum number of iterations

root = newton_solver.newton_method(ctypes.c_double(initial_guess), ctypes.c_double(tolerance), ctypes.c_int(max_iter))

print(f"Root found: {root}")

# Define the function and its graph
x = np.linspace(-10, 20, 500)
y = x**2 - 7*x - 60

# Plot the function and mark the root
plt.plot(x, y, label=r'$f(x) = x^2 - 7x - 60$')
plt.axhline(0, color='black', linewidth=0.7, linestyle='--')
plt.scatter([root], [0], color='red', label=f'Root at x = {root:.6f}')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid()
plt.show()

