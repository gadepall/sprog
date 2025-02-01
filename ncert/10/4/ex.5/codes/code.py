import math
import numpy as np
import matplotlib.pyplot as plt

# Define the quadratic function and its derivative
def f(x):
    return 3 * x**2 - 2 * math.sqrt(6) * x + 2

def f_prime(x):
    return 6 * x - 2 * math.sqrt(6)

# Define the Fixed-Point Iteration function
def g(x):
    return (1 / 3) * (2 * math.sqrt(6) - (2 / x))

# Newton-Raphson Method
def newton_raphson(x0, tol, max_iter):
    for i in range(max_iter):
        x1 = x0 - f(x0) / f_prime(x0)
        if abs(x1 - x0) < tol:
            return x1, i + 1
        x0 = x1
    return x0, max_iter

# Fixed-Point Iteration
def fixed_point_iteration(x0, tol, max_iter):
    for i in range(max_iter):
        x1 = g(x0)
        if abs(x1 - x0) < tol:
            return x1, i + 1
        x0 = x1
    return x0, max_iter

# Parameters
x0 = 1.0  # Initial guess
tolerance = 1e-6
max_iterations = 100

# Solve using Newton-Raphson
nr_root, nr_iters = newton_raphson(x0, tolerance, max_iterations)

# Solve using Fixed-Point Iteration
fp_root, fp_iters = fixed_point_iteration(x0, tolerance, max_iterations)

# Print results
print(f"Newton-Raphson Root: {nr_root}, Iterations: {nr_iters}")
print(f"Fixed-Point Root: {fp_root}, Iterations: {fp_iters}")

# Plot the function
x = np.linspace(0, 2, 500)  # Adjust range if necessary
y = 3 * x**2 - 2 * math.sqrt(6) * x + 2

plt.figure(figsize=(10, 6))
plt.plot(x, y, label=r"$f(x) = 3x^2 - 2\sqrt{6}x + 2$", color="blue")  # Raw string for LaTeX
plt.axhline(0, color="black", linestyle="--", linewidth=0.8)  # x-axis

# Mark the roots
plt.scatter([nr_root], [f(nr_root)], color="red", label="NR Root", marker="o")  # Circle marker
plt.scatter([fp_root], [f(fp_root)], color="green", label="FP Root", marker="x")  # X marker

# Labels and Legend
plt.title(r"Graph of $f(x) = 3x^2 - 2\sqrt{6}x + 2$")  # Raw string for LaTeX
plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.grid()
plt.show()
