import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load shared library
lib = ctypes.CDLL('./code.so')
lib.LUsolver.argtypes = [ctypes.c_int]
lib.LUsolver.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

# Define the system of equations
x = np.linspace(-10, 10, 1000)

# Equation 1: x + y = 9
y1 =  (2*x-2)/-3

# Equation 2: 8x - y = 0
y2 = (4*x+1)/9 

# Solve using LU decomposition
solution = lib.LUsolver(2)

# Extract the solution values for x and y
x_sol = solution[0][0]
y_sol = solution[1][0]
print("x = ",x_sol,",y = ",y_sol)

# Plot the lines
plt.plot(x, y1, color='red', label=r"$\frac{2}{\sqrt{x}} + \frac{3}{\sqrt{y}} = 2$")
plt.plot(x, y2, color='blue', label=r"$\frac{4}{\sqrt{x}} - \frac{9}{\sqrt{y}} = -1$")

# Plot the solution as a point
plt.scatter(x_sol, y_sol, color='green', label='Solution')
plt.text(x_sol, y_sol + 1, f'({x_sol:.2f}, {y_sol:.2f})', color='black', ha='center')

# Plot settings
plt.xlabel(r"$\frac{1}{\sqrt{x}}$($x_1$-variable)")
plt.ylabel(r"$\frac{1}{\sqrt{y}}$($x_2$-variable)")
plt.title("System of Linear Equations")
plt.grid(True)
plt.legend()
plt.savefig("../figs/fig.png")
plt.show()
