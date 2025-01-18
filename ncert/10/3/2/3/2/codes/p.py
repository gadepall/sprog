import numpy as np
import matplotlib.pyplot as plt

def line1(x):
    return (2 * x - 8) / 3  # Equation: 2x - 3y = 8

def line2(x):
    return (4 * x - 9) / 6  # Equation: 4x - 6y = 9

x_vals = np.linspace(-10, 10, 400)  # Define the range of x values

# Compute y values for both lines
y_vals_line1 = line1(x_vals)
y_vals_line2 = line2(x_vals)

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals_line1, label=r'$2x - 3y = 8$', color='blue')
plt.plot(x_vals, y_vals_line2, label=r'$4x - 6y = 9$', color='orange')

# Indicate that the lines are parallel
plt.text(0, -5, "Lines are parallel\nNo solution", color='red', fontsize=12, bbox=dict(facecolor='white', alpha=0.8))

# Labels, grid, and legend
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()

# Show the plot
plt.show()

