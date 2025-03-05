import numpy as np
import matplotlib.pyplot as plt


def f(x):
    return x**2 + 48*x - 324

# Derivative: f'(x) = 2x + 48
def df(x):
    return 2*x + 48

# Newton-Raphson method
def newton_raphson(x0, tol=1e-6, max_iter=100):
    x = x0
    iterations = [x]  # Store iterations for plotting

    for _ in range(max_iter):
        fx = f(x)
        dfx = df(x)

        if abs(fx) < tol:  # Convergence check
            return x, iterations

        if dfx == 0:
            raise ValueError("Derivative is zero. Method fails.")

        x -= fx / dfx  # Newton-Raphson iteration
        iterations.append(x)  # Store values

    raise ValueError("Solution did not converge.")

# Solve the equation using Newton-Raphson method
initial_guess = 5.0
try:
    root, iter_values = newton_raphson(initial_guess)
    print(f"The speed of the stream is: {root:.4f} km/h")
except ValueError as e:
    print(e)

# Plot the function
x_vals = np.linspace(-60, 20, 400)  # X range
y_vals = f(x_vals)

plt.figure(figsize=(8, 5))
plt.plot(x_vals, y_vals, label=r"$f(x) = x^2 + 48x - 324$", color='blue')
plt.axhline(0, color='black', linewidth=0.8)  # X-axis
plt.axvline(0, color='black', linewidth=0.8)  # Y-axis

# Plot Newton-Raphson iterations
plt.scatter(iter_values, f(np.array(iter_values)), color='red', marker='o', label="Newton Steps")

# Highlight the final root
plt.scatter([root], [0], color='green', s=100, marker='x', label="Root (6 km/h)")

plt.xlabel("x (Speed of Stream)")
plt.ylabel("f(x)")
plt.title("Newton-Raphson Method for Solving f(x) = 0")
plt.legend()
plt.savefig('fig.png')
plt.grid(True)
plt.show()

