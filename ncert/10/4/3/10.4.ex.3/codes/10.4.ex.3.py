import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the C shared library (ensure the .so file is in the same directory)
lib = ctypes.CDLL('./10.4.ex.3.so')

# Define argument types and return type for the C function
lib.newton_raphson.argtypes = [ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.newton_raphson.restype = ctypes.c_double

# Function to find the root using Newton-Raphson method
def find_root(x0, max_iter=100, tolerance=1e-6):
    return lib.newton_raphson(ctypes.c_double(x0), ctypes.c_int(max_iter), ctypes.c_double(tolerance))

# Function for f(x) = 2x^2 - 5x + 3
def f(x):
    return 2 * x**2 - 5 * x + 3

# Generate values for x-axis (from -2 to 3)
x_vals = np.linspace(-2, 3, 400)

# Compute the corresponding y values for f(x)
y_vals = f(x_vals)

# Plotting the function
plt.plot(x_vals, y_vals, label=r'$f(x) = 2x^2 - 5x + 3$', color='b')

# Mark the roots of the equation
root1 = 1.5  # Exact root from quadratic formula
root2 = 1     # Exact root from quadratic formula
plt.scatter([root1, root2], [0, 0], color='red', zorder=5, label="Roots")

# Adding labels and grid
plt.axhline(0, color='black', linewidth=0.5)  # x-axis
plt.axvline(0, color='black', linewidth=0.5)  # y-axis
plt.title('Plot of f(x) = 2x^2 - 5x + 3')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid(True)
plt.savefig('fig1.png')
# Show the plot
plt.show()

# Now, let's create a plot of initial guess vs solution using Newton-Raphson method

# Generate initial guesses from -2 to 3
initial_guesses = np.linspace(-2, 3, 20)
solutions = [find_root(x0) for x0 in initial_guesses]

# Plotting initial guess vs solution
plt.plot(initial_guesses, solutions, 'o-', label='Root approximation', color='green')

# Mark the exact roots on the plot
plt.axhline(root1, color='red', linestyle='--', label=f'Root 1: {root1}')
plt.axhline(root2, color='orange', linestyle='--', label=f'Root 2: {root2}')

# Adding labels and grid
plt.title('Initial Guess vs Newton-Raphson Solution')
plt.xlabel('Initial Guess (x0)')
plt.ylabel('Solution (Root Approximation)')
plt.legend()
plt.grid(True)
plt.savefig('fig2.png')
# Show the plot
plt.show()

