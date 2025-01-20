import numpy as np
import matplotlib.pyplot as plt

# Define the function f(x) = 2x^2 + x + 4
def f(x):
    return 2 * x**2 + x + 4

# Generate x values for plotting (a range of values from -3 to 3)
x_vals = np.linspace(-3, 3, 400)

# Compute the corresponding y values using the function
y_vals = f(x_vals)

# Create the plot
plt.plot(x_vals, y_vals, label=r'$f(x) = 2x^2 + x + 4$', color='b')

# Set plot labels and title
plt.xlabel('x')
plt.ylabel('f(x)')

# Show grid for better visualization
plt.grid(True)

# Display the legend
plt.legend()

# Display the plot
plt.savefig('fig.pdf')
plt.show()

