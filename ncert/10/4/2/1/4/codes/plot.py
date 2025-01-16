import numpy as np
import matplotlib.pyplot as plt

# Define the function
def f(x):
    return 2 * x**2 - x + 1/8

# Known root from the solution
root = 0.25

# Generate data for plotting
x_vals = np.linspace(-1, 1, 500)
y_vals = f(x_vals)

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label='$f(x) = 2x^2 - x + \\frac{1}{8}$', color='blue')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)  # x-axis
plt.scatter(root, f(root), color='red', label=f'Root: x = {root}', zorder=5)

# Annotate the root
plt.annotate(f"Root: ({root:.2f}, 0)", 
             xy=(root, 0), 
             xytext=(root -0.15 , 0.1), 
             fontsize=10)

# Customize the plot
plt.xlabel('$x$', fontsize=12)
plt.ylabel('$f(x)$', fontsize=12)
plt.legend(fontsize=10)
plt.grid(alpha=0.4)
plt.show()

