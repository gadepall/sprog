import numpy as np
import matplotlib.pyplot as plt

# Define the function
def f(x):
    return x**2 - 3*x - 4

# Quadratic coefficients
a, b, c = 1, -3, -4
discriminant = b**2 - 4*a*c

# Check if roots are real or complex
if discriminant >= 0:
    root1 = (-b + np.sqrt(discriminant)) / (2*a)
    root2 = (-b - np.sqrt(discriminant)) / (2*a)
    roots_real = True
else:
    root1 = (-b + np.sqrt(complex(discriminant))) / (2*a)
    root2 = (-b - np.sqrt(complex(discriminant))) / (2*a)
    roots_real = False

# Generate data for plotting
x_vals = np.linspace(-5, 5, 500)
y_vals = f(x_vals)

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label='$f(x) = x^2 -3*x -4$', color='blue')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)  # x-axis

# If real roots exist, plot them
if roots_real:
    plt.scatter([root1, root2], [0, 0], color='red', label=f'Roots: x = {root1:.2f}, x = {root2:.2f}', zorder=5)
    plt.annotate(f"Root 1: ({root1:.2f}, 0)", 
                 xy=(root1, 0), 
                 xytext=(root1 + 0.5, 5), 
                 arrowprops=dict(arrowstyle='->', color='red'), 
                 fontsize=10)

    plt.annotate(f"Root 2: ({root2:.2f}, 0)", 
                 xy=(root2, 0), 
                 xytext=(root2 - 2, -5), 
                 arrowprops=dict(arrowstyle='->', color='red'), 
                 fontsize=10)
else:
    print(f"The roots are complex: {root1}, {root2}")

# Customize the plot
plt.xlabel('$x$', fontsize=12)
plt.ylabel('$f(x)$', fontsize=12)
plt.legend(fontsize=10)
plt.grid(alpha=0.4)
plt.show()

