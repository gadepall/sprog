import numpy as np
import matplotlib.pyplot as plt

# Define the function f(x)
def f(x):
    return x**2 - 3*x - 10

# Roots of the quadratic equation, calculated using the quadratic formula
root1 = 5
root2 = -2

# Create a range of x values for plotting
x = np.linspace(-5, 8, 500)

# Calculate f(x) values
y = f(x)

# Plot the graph
plt.figure(figsize=(8, 6))
plt.plot(x, y, label=r"$f(x) = x^2 - 3x - 10$", color='blue')
plt.axhline(0, color='black', linewidth=0.8, linestyle='--', label='x-axis')  # x-axis
plt.axvline(root1, color='red', linestyle='--', label=f'Root 1: x = {root1}')
plt.axvline(root2, color='green', linestyle='--', label=f'Root 2: x = {root2}')

# Highlight roots on the graph
plt.scatter([root1, root2], [0, 0], color='red', zorder=5)

# Add labels, legend, and grid
plt.xlabel("$x$")
plt.ylabel("$f(x)$")
plt.title("Graph of $f(x) = x^2 - 3x - 10$")
plt.legend()
plt.grid(True)

# Show the plot
plt.show()

