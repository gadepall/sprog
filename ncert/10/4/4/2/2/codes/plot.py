import numpy as np
import matplotlib.pyplot as plt

# Define the function f(x) = x^2 - 2x + 1
def f(x):
    return x**2 - 2*x + 1

# Generate x values from -2 to 4 for a smooth curve
x_values = np.linspace(-2, 4, 400)

# Calculate the corresponding y values for each x
y_values = f(x_values)

# Points to highlight
highlight_x = [1, 0.99999, 1.014142]
highlight_y = [f(x) for x in highlight_x]

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label=r'Theory: $f(x) = x^2 - 2x + 1$', color='b')

# Mark the points with red dots (simulated points)
plt.scatter(highlight_x[0], highlight_y[0], color='red', zorder=5, label='theory')
plt.scatter(highlight_x[1], highlight_y[1], color='green', zorder=5, label='Sim1')
plt.scatter(highlight_x[2], highlight_y[2], color='purple', zorder=5, label='Sim2')


# Add labels and title
plt.title(r'Plot of $f(x) = x^2 - 2x + 1$', fontsize=14)
plt.xlabel('x', fontsize=12)
plt.ylabel('f(x)', fontsize=12)

# Add a grid
plt.grid(True)

# Add a legend
plt.legend()

# Display the plot
plt.show()

