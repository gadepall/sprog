import numpy as np
import matplotlib.pyplot as plt

# Define the lines
def line1(u1):
    return (2 - 5 * u1)

def line2(u1):
    return (6 * u1 - 1) / 3

# Define the range of u1
u1 = np.linspace(-1, 1, 1000)

# Compute v values for both lines
v1 = line1(u1)
v2 = line2(u1)

# Solve for the intersection point
A = np.array([[5, 1], [6, -3]])
b = np.array([2, 1])
intersection = np.linalg.solve(A, b)  # [u, v]

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(u1, v1, label='5u + v = 2', color='blue')
plt.plot(u1, v2, label='6u - 3v = 1', color='orange')

# Mark the intersection point
plt.scatter(intersection[0], intersection[1], color='red', label=f'Intersection ({intersection[0]:.2f}, {intersection[1]:.2f})', zorder=5)

# Customize labels and styling
plt.xlabel(r'$u = \frac{1}{x-1}$', fontsize=12)
plt.ylabel(r'$v = \frac{1}{y-2}$', fontsize=12)
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend(fontsize=10)

# Show the plot
plt.title("Intersection of Lines in u-v Space", fontsize=14)
plt.savefig('../figs/fig.pdf')
plt.show()

