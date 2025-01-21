import numpy as np
import matplotlib.pyplot as plt

# Define the functions for the lines
def line1(y):
    return 105 - 10 * y

def line2(y):
    return 155 - 15 * y

# Generate y values
y = np.linspace(0, 20, 400)

# Compute x values for both lines
x1 = line1(y)
x2 = line2(y)

# Intersection point
intersection_x = 5
intersection_y = 10

# Plot the lines with reversed axes
plt.figure(figsize=(8, 6))
plt.plot(x1, y, label='x + 10y = 105', color='blue')
plt.plot(x2, y, label='x + 15y = 155', color='orange')

# Highlight the intersection point
plt.scatter(intersection_x, intersection_y, color='red', label='Intersection (5, 10)', zorder=5)
plt.text(intersection_x, intersection_y, '  (5, 10)', color='red', fontsize=10, verticalalignment='bottom')

# Labels and legend
plt.xlabel('x (Fixed Charge)')
plt.ylabel('y (charge per kilometer)')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()

plt.show()

