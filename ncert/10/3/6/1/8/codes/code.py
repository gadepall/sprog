import numpy as np
import matplotlib.pyplot as plt

# Define the lines
def line1(x1):
    return (4 - 3 * x1) 

def line2(x1):
    return (3 * x1-2) 

# Define the range of u1
u1 = np.linspace(-2, 4, 1000)

# Compute v values for both lines
v1 = line1(u1)
v2 = line2(u1)

# Solve for the intersection point
A = np.array([[3, 1], [3, -1]])
b = np.array([4, 2])
intersection = np.linalg.solve(A, b)  # [u, v]

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(u1, v1, label='3x + y = 4', color='blue')
plt.plot(u1, v2, label='3x - y = 2', color='orange')

# Mark the intersection point
plt.scatter(intersection[0], intersection[1], color='red', label=f'Intersection ({intersection[0]:.2f}, {intersection[1]:.2f})', zorder=5)
plt.text(intersection[0] + 0.1, intersection[1] - 0.1, 
         f'({intersection[0]:.2f}, {intersection[1]:.2f})', 
         fontsize=12, color='red')

# Customize labels and styling
plt.xlabel(r'$u = x$', fontsize=12)
plt.ylabel(r'$v = y$', fontsize=12)
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend(fontsize=10)

# Show the plot
plt.title("Intersection of Lines in u-v Space", fontsize=14)
plt.savefig('../figs/fig.pdf')
plt.show()

