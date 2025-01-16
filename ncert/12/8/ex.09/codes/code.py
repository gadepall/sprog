import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, c_double

# Load the shared library
lib = CDLL("./code.so")

# Specify the argument and return types for the C function
lib.Area.argtypes = [c_double, c_double, c_double, c_double, c_double, c_double]
lib.Area.restype = c_double

# Vertices of the triangle (same as your original code)
x = [1.0, 2.0, 3.0, 1.0]  # Closing the triangle by repeating the first vertex
y = [0.0, 2.0, 1.0, 0.0]

# Call the C function to calculate the area of the triangle
area = lib.Area(x[0], y[0], x[1], y[1], x[2], y[2])
print(f"Triangle Area from C function: {area:.6f}")

# Create the figure and axis
plt.figure(figsize=(8, 8))

# Plot the triangle with a different style
plt.fill(x, y, color='lightblue', alpha=0.5, label='Triangle Area')  # Filled triangle
plt.plot(x, y, marker='o', linestyle='-', color='darkblue', label='Triangle Edges')  # Edges

# Annotate the vertices
for i, (xi, yi) in enumerate(zip(x, y)):
    if i < len(x) - 1:  # Exclude the closing vertex
        plt.text(xi, yi, f"({xi}, {yi})", fontsize=10, ha='center', va='bottom', color='darkblue')

# Set labels and title
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('Triangle Visualization with Filled Area')
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.legend()
plt.axis('equal')  # Equal scaling for accurate representation

# Save the plot
plt.savefig("../figure/fig.png")

# Show the plot
plt.show()

