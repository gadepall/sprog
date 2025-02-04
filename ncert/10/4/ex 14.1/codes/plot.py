import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared C library
lib = ctypes.CDLL('./libfunctions.so')

# Define the argument and return types of the C function
lib.newtonRaphson.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.newtonRaphson.restype = ctypes.c_double
lib.qr_algorithm.argtypes=[ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.c_int]
lib.qr_algorithm.restype=None
# Define the parameters for Newton-Raphson method
initial_guess_1 = 3.0
initial_guess_2=0.0
tolerance = 1e-6
max_iterations = 100
n = 2
A = np.array([[0, 1], [-1, 3]], dtype=np.double)
A_ptr = (ctypes.POINTER(ctypes.c_double) * n)()
for i in range(n):
    A_ptr[i] = (ctypes.c_double * n)(*A[i])

# Call the C function
lib.qr_algorithm(A_ptr, n)

# Extract the computed roots (diagonal elements of A)
roots = [A_ptr[i][i] for i in range(n)]
for root in roots:

    plt.scatter(root, 0, color='red', zorder=3)

    plt.text(root, 0.5, f"{root:.3f}", fontsize=12, ha='center', color='red')
# Call the C function to get the root
root_1 = lib.newtonRaphson(initial_guess_1, tolerance, max_iterations)
root_2=lib.newtonRaphson(initial_guess_2, tolerance, max_iterations)
# To visualize the root, let's plot the function f(x) = x^2 - 3x + 1

# Define the function
def f(x):
    return x**2 - 3*x + 1

# Generate x values
x_vals = np.linspace(-2, 4, 400)
y_vals = f(x_vals)

# Plot the function and the root
plt.plot(x_vals, y_vals, label=r'theory', color='blue')
plt.axhline(0, color='black',linewidth=1)
plt.axvline(root_1, color='red', linestyle='--', label='Root-1')
plt.scatter(root_1, f(root_1), color='red', zorder=3)  # Mark the root
plt.annotate(f'({root_1:.4f}, {f(root_1):.4f})', 
             xy=(root_1, f(root_1)), 
             xytext=(root_1 + 0.3, f(root_1) + 1))

plt.axvline(root_2, color='black', linestyle='--',label="Root-2")
plt.scatter(root_2, f(root_2), color='red', zorder=3)  # Mark the root with a red dot
plt.annotate(f'({root_2:.4f}, {f(root_2):.4f})', 
             xy=(root_2, f(root_2)), 
             xytext=(root_2 + 0.3, f(root_2) + 1))
for root in roots:
    plt.scatter(root, 0, color='red', zorder=3)
    plt.text(root, 0.5, f"{root:.3f}", fontsize=12, ha='center', color='red')


# Adding labels and title
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()

# Show plot
plt.grid(True)
plt.show()

