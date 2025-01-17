import numpy as np
import matplotlib.pyplot as plt

# Define the function
def f(x):
    return 2*x**2 + x - 6

# Calculate the roots using the quadratic formula
a, b, c = 2, 1, -6
discriminant = b**2 - 4*a*c
root1 = (-b + np.sqrt(discriminant)) / (2*a)
root2 = (-b - np.sqrt(discriminant)) / (2*a)

# Generate data for plotting
x_vals = np.linspace(-5, 5, 500)
y_vals = f(x_vals)

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label='$f(x) = 2x^2 + x - 6$', color='blue')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)  # x-axis
plt.scatter([root1, root2], [f(root1), f(root2)], color='red', label=f'Roots: x = {root1:.2f}, x = {root2:.2f}', zorder=5)

# Annotate the roots
plt.annotate(f"Root 1: ({root1:.2f}, 0)", 
             xy=(root1, 0), 
             xytext=(root1 + 0.5, 5), 
             arrowprops=dict(arrowstyle='->', color='red'), 
             fontsize=10)

plt.annotate(f"Root 2: ({root2:.2f}, 0)", 
             xy=(root2, 0), 
             xytext=(root2 - 2, -5), 
             arrowprops=dict(arrowstyle='->', color='red'), 
             fontsize=10)

# Customize the plot
plt.xlabel('$x$', fontsize=12)
plt.ylabel('$f(x)$', fontsize=12)
plt.legend(fontsize=10)
plt.grid(alpha=0.4)
plt.show()

