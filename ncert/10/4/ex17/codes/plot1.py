import numpy as np
import matplotlib.pyplot as plt

# Define the quadratic equation
def quadratic(x):
    return x**2 + 7 * x - 60

# Roots of the quadratic equation
roots = [5, -12]

# Generate x values for the plot
x = np.linspace(-20, 20, 500)
y = quadratic(x)

# Plot the quadratic function
plt.figure(figsize=(8, 6))
plt.plot(x, y, label=r'$x^2 + 7x - 60$', color='blue')

# Highlight the root at x = 5 more prominently
plt.scatter(5, 0, color='green', s=150, zorder=5, edgecolor='black', label='Positive Root at x = 5')

# Add labels, grid, and legend
plt.axhline(0, color='black', linewidth=0.8, linestyle='--')
plt.axvline(0, color='black', linewidth=0.8, linestyle='--')
plt.title('Plot of Quadratic Equation and Its Roots')
plt.xlabel('x')
plt.ylabel('y')
plt.grid(True)
plt.legend()
plt.show()
