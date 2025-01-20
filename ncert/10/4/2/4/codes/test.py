import numpy as np
import matplotlib.pyplot as plt
import os
from ctypes import *

# Get the directory where the current code is located
script_dir = os.path.dirname(os.path.abspath(__file__))
# path we want to save this figure 
save_directory = os.path.join(script_dir, "../fig")

if not os.path.exists(save_directory):
    os.makedirs(save_directory)  # creates directory if not exists

solve = CDLL('./test.so')  # loads all the functions that are in test.c

# Define the function and its derivative
solve.newton.restype = c_double  # return datatype 
solve.secant.restype = c_double  # return datatype of generate_xpoints
def f(x):
    return x**2 + x - 182

def f_prime(x):
    return 2*x + 1

# Newton's Method
def newton_method(x0, tol=1e-6, max_iter=100):
    for _ in range(max_iter):
        x_new = solve.newton(c_double(x0),c_double(f(x0)),c_double(f_prime(x0)))
        print(x_new)
        if abs(x_new - x0) < tol:
            break
        x0 = x_new
    return x_new

# Secant Method
def secant_method(x0, x1, tol=1e-6, max_iter=100):
    for _ in range(max_iter):
        if f(x1) - f(x0) == 0:
            break
        x_new = solve.secant(c_double(x1),c_double(f(x1)),c_double(x0),c_double(f(x0)))
        if abs(x_new - x1) < tol:
            break
        x0, x1 = x1, x_new
    return x_new

# Initial guesses for Newton's and Secant methods
x0_newton = 13.5  # Initial guess for Newton's Method
x0_secant = 13.5    # First initial guess for Secant Method
x1_secant = 14    # Second initial guess for Secant Method

# Run the methods
newton_root = newton_method(x0_newton)
secant_root = secant_method(x0_secant, x1_secant)

# Generate x values for plotting the parabola
theoretical_x = np.linspace(10, 15, 100)
theoretical_y = f(theoretical_x)

# Plot the parabola and convergence paths
plt.figure(figsize=(10, 6))

# Plot the theoretical parabola
plt.plot(theoretical_x, theoretical_y, label="Theoretical Parabola", color="blue")

# Plot Newton's method final approximation (root)
plt.scatter(newton_root, f(newton_root), color="yellow", label="Newton's Method Root",s = 100, zorder=5)

# Plot Secant method final approximation (root)
plt.scatter(secant_root, f(secant_root), color="red", label="Secant Method Root", zorder=5)

# Add labels, legend, and grid
plt.axhline(0, color="black", linewidth=0.8, linestyle="--")
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Parabola with Newton\'s and Secant Methods')
plt.legend()
plt.grid()
save_path = os.path.join(save_directory, 'combined_fig.jpg')  # saves as combined_fig.jpg
plt.savefig(save_path)
plt.show()

# Print the results
print("Newton's Method Root:", newton_root)
print("Secant Method Root:", secant_root)

