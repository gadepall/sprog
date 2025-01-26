import numpy as np
import matplotlib.pyplot as plt

# Define the equations
def line1(x):
    return 25 - x

def line2(x):
    return (2000 - 50 * x) / 100

# Generate x values for the plot
x_vals = np.linspace(0, 30, 500)

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x_vals, line1(x_vals), label=r"$x + y = 25$", color="blue")
plt.plot(x_vals, line2(x_vals), label=r"$50x + 100y = 2000$", color="red")

# Highlight the intersection point
plt.scatter(10, 15, color="green", label="Intersection Point (10, 15)", zorder=5)
plt.text(10, 15.5, "(10, 15)", fontsize=10, color="green")

# Add labels, legend, and grid
plt.title("Graphical Solution of Linear Equations")
plt.xlabel("Number of ₹50 Notes (x)")
plt.ylabel("Number of ₹100 Notes (y)")
plt.axhline(0, color="black", linewidth=0.8, linestyle="--")
plt.axvline(0, color="black", linewidth=0.8, linestyle="--")
plt.grid(color="gray", linestyle="--", linewidth=0.5)
plt.legend()
plt.show()
