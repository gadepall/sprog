import numpy as np
import matplotlib.pyplot as plt

# Define the equations as functions
def equation1(x):
    return (50 - 5 * x) / 7  # Rearrange 5x + 7y = 50 to y = (50 - 5x)/7

def equation2(x):
    return (46 - 7 * x) / 5  # Rearrange 7x + 5y = 46 to y = (46 - 7x)/5

# Generate x values for plotting
x = np.linspace(0, 10, 500)

# Plot the equations
plt.plot(x, equation1(x), label=r'$5x + 7y = 50$', color='blue')
plt.plot(x, equation2(x), label=r'$7x + 5y = 46$', color='orange')

# Solution point
solution_x = 3
solution_y = 5
plt.scatter(solution_x, solution_y, color='red', label=f'Solution: (x={solution_x}, y={solution_y})')

# Add labels
plt.xlabel('x (Cost of a pencil)')
plt.ylabel('y (Cost of a pen)')
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')

# Add legend
plt.legend()
plt.grid(True)
plt.show()

