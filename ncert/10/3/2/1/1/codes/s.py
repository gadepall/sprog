import numpy as np
import matplotlib.pyplot as plt

# Create an array of x values
x_values = np.linspace(-10, 10, 400)

# Equation of the first line: x + y = 10 => y = 10 - x
y1_values = 10 - x_values

# Equation of the second line: y = x + 4
y2_values = x_values + 4

# Intersection point
intersection_x = 3
intersection_y = 7

# Create the plot
plt.figure(figsize=(6, 6))

# Plot the first line
plt.plot(x_values, y1_values, label=r'$x + y = 10$', color='blue')

# Plot the second line
plt.plot(x_values, y2_values, label=r'$y = x + 4$', color='red')

# Plot the intersection point
plt.scatter(intersection_x, intersection_y, color='green', zorder=5, label=f'Intersection (3, 7)')

# Add labels and title
plt.xlabel('x')
plt.ylabel('y')


# Add grid and legend
plt.grid(True)
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.legend()

# Show the plot
plt.show()

