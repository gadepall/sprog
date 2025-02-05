import numpy as np
import matplotlib.pyplot as plt

# Define the lines based on the equations x - 3y = -10 and x - 2y = 10
def line1(y):
    return -10 + 3 * y  # Rearranged: x = -10 + 3y

def line2(y):
    return 10 + 2 * y   # Rearranged: x = 10 + 2y

# Define the range of y
y = np.linspace(-10, 30, 1000)

# Compute x values for both lines
x1 = line1(y)
x2 = line2(y)

# Solve for the intersection point
A = np.array([[1, -3], [1, -2]])  # Coefficient matrix
b = np.array([-10, 10])           # Constants vector
intersection = np.linalg.solve(A, b)  # [x, y]

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x1, y, label='x - 3y = -10', color='blue')
plt.plot(x2, y, label='x - 2y = 10', color='orange')

# Mark the intersection point
plt.scatter(intersection[0], intersection[1], color='red', label=f'Intersection ({intersection[0]:.2f}, {intersection[1]:.2f})', zorder=5)

# Customize labels and styling
plt.xlabel('x', fontsize=12)
plt.ylabel('y', fontsize=12)
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend(fontsize=10)

# Show the plot
plt.title("Intersection of Lines in x-y Space", fontsize=14)
plt.savefig('fig.pdf')  # Save the figure as a PDF
plt.show()
