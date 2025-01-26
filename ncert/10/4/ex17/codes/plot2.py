import numpy as np
import matplotlib.pyplot as plt

# Define the quadratic function and its derivative
def f(x):
    return x**2 + 7*x - 60

def f_prime(x):
    return 2*x + 7

# Newton's Method implementation
def newtons_method(x0, max_iter=10, tol=1e-6):
    roots = [x0]  # Initial guess
    for _ in range(max_iter):
        x1 = x0 - f(x0) / f_prime(x0)
        roots.append(x1)
        if abs(x1 - x0) < tol:
            break
        x0 = x1
    return roots

# Generate the Newton's method iterations
initial_guess = -10  # Starting point for Newton's method
roots = newtons_method(initial_guess)

# Create a grid for the function's contour plot
x_real = np.linspace(-20, 20, 400)
x_imag = np.linspace(-2, 2, 400)
X, Y = np.meshgrid(x_real, x_imag)
Z = f(X + 1j * Y)

# Plot the contour
plt.figure(figsize=(10, 8))
contour = plt.contour(X, Y, np.abs(Z), levels=50, cmap='viridis')
plt.colorbar(contour, label=r'$|f(x)|$')

# Plot the Newton's method iterations
for i, root in enumerate(roots):
    color = 'orange' if not np.isclose(root, 5, atol=1e-2) else 'blue'
    size = 50 if not np.isclose(root, 5, atol=1e-2) else 150
    plt.scatter(root, 0, color=color, s=size, edgecolor='black' if np.isclose(root, 5, atol=1e-2) else None,
                label=f'Newton Iter {i}: ({root:.2f})' if i == 0 else "")
    if not np.isclose(root, 5, atol=1e-2):
        plt.text(root, 0.2, f'{i}', color='orange', fontsize=10)

# Highlight the root x = 5 prominently
plt.scatter(5, 0, color='blue', s=200, edgecolor='black', zorder=5, label='Root x = 5')
plt.text(5, 0.5, 'Root x = 5', color='blue', fontsize=12, ha='center')

# Add labels and legend
plt.axhline(0, color='black', linewidth=0.8, linestyle='--')
plt.axvline(0, color='black', linewidth=0.8, linestyle='--')
plt.title("Newton's Method for Quadratic Equation: $f(x) = x^2 + 7x - 60$")
plt.xlabel('Real Part of x')
plt.ylabel('Imaginary Part of x')
plt.legend(loc='upper left', fontsize=10)
plt.grid(True)

# Show the plot
plt.show()
