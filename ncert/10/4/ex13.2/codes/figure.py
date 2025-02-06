import numpy as np
import matplotlib.pyplot as plt

# Define the quadratic function
def quadratic(x):
    return x**2 + 4*x + 5

# Generate x values
x = np.linspace(-6, 2, 400)
y = quadratic(x)

# Plot the quadratic function
plt.plot(x, y, label='y = x² + 4x + 5', color='blue')

# Labels and legend
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid()

# Show the plot
plt.show()
