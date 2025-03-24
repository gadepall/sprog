import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load shared library
lib = ctypes.CDLL('./lu.so')
lib.LUsolver.argtypes = [ctypes.c_int]
lib.LUsolver.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

# Define the system of equations
x = np.linspace(-10, 10, 1000)

# Equation 1: (x/2) + (2y/3) = -1  -> Solve for y
y1 = (3 * (-1 - x / 2)) / 2

# Equation 2: x - (y/3) = 3  -> Solve for y
y2 = 3 * (x - 3)

# Solve using LU decomposition
solution = lib.LUsolver(2)

# Extract the solution values for x and y
x_sol = solution[0][0]
y_sol = solution[1][0]
print("x = ", x_sol, ", y = ", y_sol)

# Plot the lines
plt.plot(x, y1, color='red', label=r"$\frac{x}{2} + \frac{2y}{3} = -1$")
plt.plot(x, y2, color='blue', label=r"$x - \frac{y}{3} = 3$")

# Plot the solution as a point
plt.scatter(x_sol, y_sol, color='green', label="Solution")
plt.text(x_sol, y_sol + 1, rf"$({x_sol:.2f}, {y_sol:.2f})$", color='black', ha='center')

# Plot settings
plt.xlabel(r"$x$")
plt.ylabel(r"$y$")
plt.title("System of Linear Equations")
plt.grid(True)
plt.legend()
plt.show()

