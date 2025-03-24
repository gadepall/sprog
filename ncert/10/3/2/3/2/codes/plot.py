import numpy as np
import matplotlib.pyplot as plt

# Define the x values
x = np.linspace(-10, 10, 400)

# Define the two equations and solve for y
y1 = (2 * x - 8) / 3
y2 = (4 * x - 9) / 6

# Plot the two equations
plt.plot(x, y1, label=r'$2x - 3y = 8$', color='blue')
plt.plot(x, y2, label=r'$4x - 6y = 9$', color='red')

# Add labels and title
plt.xlabel('x')
plt.ylabel('y')
plt.title('Plot of the equations: 2x - 3y = 8 and 4x - 6y = 9')

# Add a legend
plt.legend()

# Set grid
plt.grid(True)

# Save the plot as a PNG file
plt.savefig("../figs/Figure_1.png")

# Show the plot
plt.show()

