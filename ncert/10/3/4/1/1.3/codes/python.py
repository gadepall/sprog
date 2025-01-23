import numpy as np
import matplotlib.pyplot as plt
import ctypes

# DLL linking: Linking the '.so' file coded from 'C' dynamically to our Python program
dll = ctypes.CDLL('./libsolver.so')

# Specifying argument and return types for the LUsolver function defined in the C program
dll.LUsolver.argtypes = [ctypes.c_int]
dll.LUsolver.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

# Plotting the lines for the given equations
x = np.linspace(-10, 10, 1000)

# Equation 1: 3x - 5y - 4 = 0 -> y = (3x - 4)/5
y1 = (3 * x - 4) / 5

# Equation 2: 9x = 2y + 7 -> y = (9x - 7) / 2
y2 = (9 * x - 7) / 2

# Plot the lines
plt.plot(x, y1, color='r', label="3x - 5y = 4")
plt.plot(x, y2, color='b', label="9x - 2y = 7")

# Verifying with LU decomposition
# Call the LUsolver function from the C library
solution = dll.LUsolver(2)

# Extract the solution values for x and y
x_sol = solution[0][0]
y_sol = solution[1][0]

# Plot the solution as a point
plt.scatter(x_sol, y_sol, color='g', label='Solution')

# Plot settings
plt.xlabel("x")
plt.ylabel("y")
plt.title("System of Linear Equations")
plt.grid(True)
plt.legend()
plt.savefig("solution_plot.png")

