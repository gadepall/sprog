import numpy as np
import matplotlib.pyplot as plt

# Define the quadratic function
def quadratic(x):
    return 2 * x**2 + x - 4

# Calculate the roots using the quadratic formula
a, b, c = 2, 1, -4
discriminant = b**2 - 4 * a * c

if discriminant >= 0:
    root1 = (-b + np.sqrt(discriminant)) / (2 * a)
    root2 = (-b - np.sqrt(discriminant)) / (2 * a)
    roots = [root1, root2]
else:
    roots = []

# Generate x values and corresponding y values for the plot
x = np.linspace(-3, 3, 400)
y = quadratic(x)

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(x, y, label='$y=2x^2+x-4$', color='blue')
plt.axhline(0, color='black', linewidth=0.8, linestyle='--')  # x-axis

# Highlight and label the roots
if roots:
    for root in roots:
        plt.scatter(root, 0, color='red', zorder=5)
        plt.text(root, 0.5, f'({root:.2f}, 0)', fontsize=10, color='red')


plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()

# Show the plot



plt.savefig("../figs/Figure_1.png")
plt.show()

