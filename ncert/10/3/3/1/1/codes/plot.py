import numpy as np
import matplotlib.pyplot as plt

# Define the equations as functions
def line1(x):
    return 14 - x  # Rearranged x + y = 14 to y = 14 - x

def line2(x):
    return x - 4  # Rearranged x - y = 4 to y = x - 4

# Generate x values for the plot
x = np.linspace(0, 15, 400)

# Compute y values for both lines
y1 = line1(x)
y2 = line2(x)

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x, y1, label='x + y = 14', color='blue')
plt.plot(x, y2, label='x - y = 4', color='orange')

# Calculate the intersection point (solution to the system of equations)
x_intersect = (14 + 4) / 2  # Solving x + y = 14 and x - y = 4 for x
y_intersect = 14 - x_intersect  # Substitute x into x + y = 14

# Mark the intersection point
plt.scatter(x_intersect, y_intersect, color='red', label=f'Solution: (x, y) = ({x_intersect}, {y_intersect})')

# Labels, legend, and grid
plt.xlabel('x (variable 1)')
plt.ylabel('y (variable 2)')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()
plt.show()

