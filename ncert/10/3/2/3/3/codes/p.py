import numpy as np
import matplotlib.pyplot as plt

# Generate x values for plotting
x_vals = np.linspace(-10, 10, 100)  # Range of x values

# Line 1: y = (21/5) - (9/10)x
y_vals_line1 = (21/5) - (9/10) * x_vals

# Line 2: y = (7/5) - (9/10)x
y_vals_line2 = (7/5) - (9/10) * x_vals

# Plot the lines
plt.plot(x_vals, y_vals_line1, label=r'$\frac{3}{2}x + \frac{5}{3}y = 7$', color='blue')
plt.plot(x_vals, y_vals_line2, label=r'$9x + 10y = 14$', color='red')

# Add labels and grid
plt.xlabel('X')
plt.ylabel('Y')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.legend()
plt.grid(True)

# Show the plot
plt.show()

