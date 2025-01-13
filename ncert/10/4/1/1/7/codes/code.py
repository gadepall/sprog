import numpy as np
import matplotlib.pyplot as plt

# Define the function
def f(x):
    return x**3 - 2*x**2 - 6*x - 8

root = 4.0 

# Generate data for plotting
x_vals = np.linspace(-5, 5, 500)
y_vals = f(x_vals)

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label='$f(x) = x^3 - 2x^2 - 6x - 8$', color='blue')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)  # x-axis
plt.scatter(root, f(root), color='red', label=f'Root: x = {root}', zorder=5)

# Annotate the root
plt.annotate(f"Root: ({root:.2f}, 0)", 
             xy=(root, 0), 
             xytext=(root + 1, 10), 
             arrowprops=dict(arrowstyle='->', color='red'), 
             fontsize=10)

# Customize the plot
plt.xlabel('$x$', fontsize=12)
plt.ylabel('$f(x)$', fontsize=12)
plt.legend(fontsize=10)
plt.grid(alpha=0.4)
plt.show()

