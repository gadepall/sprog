#Code by S.Sai Akshita
#importing necessary libraries
import numpy as np
import matplotlib.pyplot as plt

# Define the range of x values
x = np.linspace(-10, 10, 400)

# Define the equations
# Equation 1: 3x - 5y = 20  => y = (3x - 20) / 5
y1 = (3 * x - 20) / 5

# Equation 2: 6x - 10y = 40 => y = (6x - 40) / 10
# Simplifies to the same line as Equation 1
y2 = (6 * x - 40) / 10

# Plot the equations
plt.figure(figsize=(8, 6))
plt.plot(x, y1, label=r"$3x - 5y = 20 $", color="blue")
plt.plot(x, y2, label=r"$6x - 10y = 40$", color="red", linestyle="--")

# Add labels, legend, and title
plt.axhline(0, color='black', linewidth=0.5, linestyle="--")  # x-axis
plt.axvline(0, color='black', linewidth=0.5, linestyle="--")  # y-axis
plt.xlabel("x")
plt.ylabel("y")
plt.title("Plot of Linear Equations")
plt.legend(loc="upper left")

# Set grid and limits
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.xlim(-10, 10)
plt.ylim(-10, 10)

# Show the plot
plt.savefig('lines.png')
plt.show()
