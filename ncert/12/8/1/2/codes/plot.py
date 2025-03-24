import numpy as np
import matplotlib.pyplot as plt

# Define the function y^2 = 9x, so y = sqrt(9x)
def y(x):
    return np.sqrt(9 * x)

# Define the x range between 2 and 4
x = np.linspace(2, 4, 400)
y_values = y(x)

# Plot the curve y^2 = 9x
plt.plot(x, y_values, label=r'$y^2 = 9x$', color='blue')

# Fill the area between the curve and x-axis
plt.fill_between(x, y_values, color='lightblue', alpha=0.5)

# Adding vertical lines for x=2 and x=4
plt.axvline(x=2, color='red', linestyle='--', label=r'$x=2$')
plt.axvline(x=4, color='green', linestyle='--', label=r'$x=4$')

# Set the limits of the plot (increase y-axis limit)
plt.xlim([0, 5])
plt.ylim([0, 8])  # Increased the upper limit of the y-axis

# Add labels and a title
plt.xlabel('x')
plt.ylabel('y')

# Add a legend
plt.legend()

# Display the plot
plt.grid(True)
plt.savefig("Figure_1.png")

