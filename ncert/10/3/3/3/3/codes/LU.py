import numpy as np
import matplotlib.pyplot as plt

# Defining the coefficients of the equations
A = np.array([[7, 6], [3, 5]])
B = np.array([3800, 1750])

# Solve the system of equations
solution = np.linalg.solve(A, B)
x_value, y_value = solution

x_values = np.linspace(0, 1000, 500)
y_values_1 = (3800 - 7*x_values) / 6
y_values_2 = (1750 - 3*x_values) / 5

# Plot the equations
plt.plot(x_values, y_values_1, label="7x + 6y = 3800")
plt.plot(x_values, y_values_2, label="3x + 5y = 1750")

# Plot the solution point
plt.scatter(x_value, y_value, color='red', label=f"Solution: x={x_value:.2f}, y={y_value:.2f}")

plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.show()

