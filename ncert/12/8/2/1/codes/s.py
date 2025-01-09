import numpy as np
import matplotlib.pyplot as plt

# Define the parabola: y = x^2 / 4
def parabola(x):
    return x**2 / 4

# Define the circle: x^2 + y^2 = 9/4
def circle_full(theta):
    r = 3 / 2  # Radius of the circle (sqrt(9/4))
    x = r * np.cos(theta)  # Parametric equation for x
    y = r * np.sin(theta)  # Parametric equation for y
    return x, y

# Compute area using the trapezoidal rule
def compute_area_between_curves(a, b, n):
    h = (b - a) / n  # Step size
    x = np.linspace(a, b, n + 1)  # Generate n+1 points in the interval [a, b]
    y_diff_upper = np.sqrt(9 / 4 - x**2) - parabola(x)  # Difference between circle and parabola
    area_upper = (h / 2) * np.sum(y_diff_upper[:-1] + y_diff_upper[1:])  # Trapezoidal rule
    return area_upper  # Total area for upper and lower combined

# Define parameters
a = -np.sqrt(2)  # Left intersection point
b = np.sqrt(2)   # Right intersection point
n = 1000  # Number of trapezoids

# Compute total area
total_area = compute_area_between_curves(a, b, n)

# Print the total area
print(f"Total area between the curves: {total_area:.4f}")

# Plotting the curves and the area
theta = np.linspace(0, 2 * np.pi, 500)  # Full circle parameter
x_circle, y_circle = circle_full(theta)

x = np.linspace(-2, 2, 500)
y_parabola = parabola(x)

plt.figure(figsize=(8, 6))
plt.plot(x, y_parabola, label=r"$x^2 = 4y$", color="blue", linewidth=2)
plt.plot(x_circle, y_circle, label=r"$x^2 + y^2 = \frac{9}{4}$", color="green", linewidth=2)

# Fill the areas between the circle and parabola
y_circle_upper = np.sqrt(9 / 4 - x**2)
y_circle_lower = -y_circle_upper
plt.fill_between(x, y_parabola, y_circle_upper, where=(y_circle_upper > y_parabola), color="lightblue", alpha=0.5)
plt.fill_between(x, y_circle_lower, -y_parabola, where=(-y_circle_lower < -y_parabola), color="lightblue", alpha=0.5)


plt.axhline(0, color="black", linewidth=0.5)
plt.xlabel("x", fontsize=12)
plt.ylabel("y", fontsize=12)
plt.legend(fontsize=12)
plt.grid(True)
plt.axis('equal')  # Maintain aspect ratio
plt.show()

