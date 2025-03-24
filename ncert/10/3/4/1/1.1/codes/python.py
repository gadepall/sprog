import numpy as np
import matplotlib.pyplot as plt

# Define the range for x values
x = np.linspace(-10, 10, 500)

# Define the equations for y in terms of x
y1 = 5 - x  # From x + y = 5
y2 = (2 * x - 4) / 3  # From 2x - 3y = 4

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x, y1, label="x + y = 5", color='blue')
plt.plot(x, y2, label="2x - 3y = 4", color='green')

# Highlight the intersection point
A = np.array([[1, 1], [2, -3]])  # Coefficients matrix
B = np.array([5, 4])  # Constants matrix
intersection = np.linalg.solve(A, B)  # Solve Ax = B
plt.scatter(intersection[0], intersection[1], color='red', label=f"Intersection {intersection}")

# Add labels, legend, and grid
plt.axhline(0, color='black',linewidth=0.5, linestyle="--")
plt.axvline(0, color='black',linewidth=0.5, linestyle="--")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.savefig('fig.pdf')
# Show the plot
plt.show()

