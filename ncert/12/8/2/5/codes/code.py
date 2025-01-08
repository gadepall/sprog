import numpy as np
import matplotlib.pyplot as plt

# Define the lines as x = ay + b
lines = [
    (1/2, -1/2),  # Line 1: x = (1/2)y - 1/2
    (1/3, -1/3),  # Line 2: x = (1/3)y - 1/3
    (0, 4)        # Line 3: x = 4
]

# Function to calculate intersection y-coordinate
def intersection_y(l1, l2):
    return (l2[1] - l1[1]) / (l1[0] - l2[0])

# Function to calculate x-coordinate for a line at a given y
def compute_x(line, y):
    return line[0] * y + line[1]

# Calculate intersection points
y1 = intersection_y(lines[0], lines[1])
x1 = compute_x(lines[0], y1)

y2 = intersection_y(lines[1], lines[2])
x2 = compute_x(lines[1], y2)

y3 = intersection_y(lines[2], lines[0])
x3 = compute_x(lines[2], y3)

# Collect intersection points
points = np.array([[x1, y1], [x2, y2], [x3, y3]])

# Plot the lines
y_range = np.linspace(min(points[:, 1]) - 1, max(points[:, 1]) + 1, 500)
for i, (a, b) in enumerate(lines):
    plt.plot(a * y_range + b, y_range, label=f'Line {i+1}: x = {a:.2f}y + {b:.2f}')

# Plot the triangle
triangle = plt.Polygon(points, color='orange', alpha=0.5, label='Triangle Area')
plt.gca().add_patch(triangle)

# Highlight the intersection points
plt.scatter(points[:, 0], points[:, 1], color='red', zorder=5, label='Intersection Points')

# Adjust plot settings
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')
plt.xlabel('x-axis')
plt.ylabel('y-axis')
plt.legend()
plt.grid()
plt.axis('equal')
plt.savefig('fig1.png')
# Show the plot
plt.show()
