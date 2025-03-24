import numpy as np
import matplotlib.pyplot as plt

def line1(x1):
    return (160 - 2 * x1) / 1

def line2(x1):
    return (300 - 4 * x1) / 2

x1 = np.linspace(0, 100, 400)

# Compute y values for both lines
y1 = line1(x1)
y2 = line2(x1)

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x1, y1, label='2x + y = 160', color='blue')
plt.plot(x1, y2, label='4x + 2y = 300', color='orange')

# Indicate that the lines are parallel
plt.text(50, 60, "Lines are parallel\nNo solution", color='red', fontsize=12, bbox=dict(facecolor='white', alpha=0.8))

# Labels and legend
plt.xlabel('x1 (Price of a apples)')
plt.ylabel('x2 (Price of a grapes)')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()

plt.show()

