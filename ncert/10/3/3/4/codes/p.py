import numpy as np
import matplotlib.pyplot as plt

# Define the equations as functions
def line1(x):
    return 180 - x  # Rearranged x + y = 180 to y = 180 - x

def line2(x):
    return x - 18  # Rearranged x - y = 18 to y = x - 18

# Generate x values for the plot with a larger range
x = np.linspace(50, 120, 400)  # Adjusted range to capture the intersection

# Compute y values for both lines
y1 = line1(x)
y2 = line2(x)

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x, y1, label='x + y = 180', color='blue')
plt.plot(x, y2, label='x - y = 18', color='orange')

# Calculate the intersection point (solution to the system of equations)
x_intersect = (180 + 18) / 2  # Solving x + y = 180 and x - y = 18 for x
y_intersect = 180 - x_intersect  # Substitute x into x + y = 180

# Mark the intersection point
plt.scatter(x_intersect, y_intersect, color='red', label=f'Solution: (x, y) = ({x_intersect:.2f}, {y_intersect:.2f})')

# Labels, legend, and grid
plt.xlabel('x (variable 1)')
plt.ylabel('y (variable 2)')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()
plt.title('Intersection of Two Lines')
plt.show()

