import numpy as np
import matplotlib.pyplot as plt

# Define the function f(x)
def f(x):
    return 3*x**2 - 4*np.sqrt(3)*x + 4

# Root of the quadratic equation
root = 2 * np.sqrt(3) / 3
x = np.linspace(-1, 3, 500)
y = f(x)

# Plot the graph
plt.figure(figsize=(8, 6))
plt.plot(x, y, label=r"$f(x) = 3x^2 - 4\sqrt{3}x + 4$", color='blue')
plt.axhline(0, color='black', linewidth=0.8, linestyle='--', label='x-axis')
plt.axvline(root, color='red', linestyle='--', label=f'Root: x = {root:.6f}')

# Highlight root on the graph
plt.scatter([root], [0], color='red', zorder=5)

plt.xlabel("$x$")
plt.ylabel("$f(x)$")
plt.legend()
plt.grid(True)
plt.show()

