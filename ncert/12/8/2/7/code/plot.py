import numpy as np
import matplotlib.pyplot as plt

# Define the range of x values
x = np.linspace(0, 4, 400)

# Define the equations
y1_positive = np.sqrt(4*x)   # Positive branch of y^2 = 4x
y1_negative = -np.sqrt(4*x)  # Negative branch of y^2 = 4x
y2 = 2*x                      # y = 2x

# Find the intersection points analytically
# For y = 2x and y^2 = 4x, substitute y = 2x into y^2 = 4x
# This gives 4x^2 = 4x, which simplifies to x(x-1) = 0
# So, x = 0 or x = 1. Then, y = 2x, so the points are (0, 0) and (1, 2)

intersection_x = [0, 1]
intersection_y = [2*xi for xi in intersection_x]

# Plot the curves
plt.figure(figsize=(8, 6))

# Plot the parabola y^2 = 4x
plt.plot(x, y1_positive, label=r"$y^2 = 4x$", color="blue")
plt.plot(x, y1_negative, color="blue")

# Plot the line y = 2x
plt.plot(x, y2, label=r"$y = 2x$", color="red")

# Correct condition to shade the region between the curves
# Using np.logical_and to combine conditions element-wise
shade_condition = np.logical_and(y2 >= y1_negative, y2 <= y1_positive)

# Shade the region bounded by the curves
plt.fill_between(x, y1_positive, y2, where=shade_condition, color='gray', alpha=0.5, label="Shaded Region")

# Plot the intersection points
plt.scatter(intersection_x, intersection_y, color='black', zorder=5)
for (xi, yi) in zip(intersection_x, intersection_y):
    plt.text(xi + 0.1, yi, f"({xi}, {yi})", fontsize=12, color='black')

# Labels and title
plt.xlabel("x-axis")
plt.ylabel("y-axis")

# Make the axes thicker by increasing the linewidth
plt.axhline(0, color='black', linewidth=2)  # Thicker x-axis
plt.axvline(0, color='black', linewidth=2)  # Thicker y-axis

# Add legend
plt.legend()

# Show plot
plt.grid(True)
plt.show()

