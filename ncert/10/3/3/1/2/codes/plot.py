import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, Eq, solve

# Define the two lines:
# Line 1: x - y = 3 --> y = x - 3
# Line 2: x/3 + y/2 = 6 --> y = 12 - (2/3)x

# Create a range of x values for plotting
x_vals = np.linspace(-10, 15, 400)

# Calculate y values for each line
y_vals_line1 = x_vals - 3
y_vals_line2 = 12 - (2/3) * x_vals

# Find the intersection point
x, y = symbols('x y')
eq1 = Eq(x - y, 3)
eq2 = Eq(x/3 + y/2, 6)
intersection = solve((eq1, eq2), (x, y))

# Extract the intersection coordinates
x_intersection, y_intersection = intersection[x], intersection[y]

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals_line1, label=r'$s - t = 3$', color='blue')
plt.plot(x_vals, y_vals_line2, label=r'$\frac{s}{3} + \frac{t}{2} = 6$', color='red')

# Highlight the intersection point
plt.scatter(x_intersection, y_intersection, color='green', zorder=5, label=f'Intersection ({x_intersection:.2f}, {y_intersection:.2f})')

# Set plot limits for better clarity
plt.xlim(-5, 15)
plt.ylim(-5, 15)
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.grid(True)

# Add labels and legend
plt.xlabel('x')
plt.ylabel('y')
plt.legend()

# Show the plot
plt.show()

