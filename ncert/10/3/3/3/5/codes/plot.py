import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, Eq, solve

# Define the equations
def eq1(x):
    return (11*x + 4) / 9

def eq2(x):
    return (6*x + 3) / 5

# Generate x values
x_vals = np.linspace(-10, 10, 400)
y_vals_1 = eq1(x_vals)
y_vals_2 = eq2(x_vals)

# Solve for the intersection point
x, y = symbols('x y')
eq1 = Eq(11*x - 9*y, -4)
eq2 = Eq(6*x - 5*y, -3)

solution = solve((eq1, eq2), (x, y))
intersection_x, intersection_y = solution[x], solution[y]

# Plot the lines
plt.plot(x_vals, y_vals_1, label=r'$11x - 9y = -4$', color='blue')
plt.plot(x_vals, y_vals_2, label=r'$6x - 5y = -3$', color='red')

# Plot intersection point
plt.scatter(float(intersection_x), float(intersection_y), color='green', zorder=5)
plt.text(float(intersection_x), float(intersection_y), f'({float(intersection_x):.2f}, {float(intersection_y):.2f})', fontsize=12, color='green')

# Labels and title
plt.xlabel('x')
plt.ylabel('y')
plt.axhline(0, color='black',linewidth=1)
plt.axvline(0, color='black',linewidth=1)
plt.title('Graph of the system of equations')
plt.legend()

# Show the plot
plt.grid(True)
plt.show()

# Print the intersection point
print(f"Intersection Point: ({float(intersection_x):.2f}, {float(intersection_y):.2f})")

