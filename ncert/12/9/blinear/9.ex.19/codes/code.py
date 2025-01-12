import numpy as np
import matplotlib.pyplot as plt
import math
import ctypes

# Parameters for the first code
h = 0.01  # Step size
n = 300   # Number of steps
a = (4 / h**2) + 1
c = a
b = 2 - (8 / h**2)
d = (2 / h) - 1
e = -(2 / h) - 1

# Initialize arrays
x = np.zeros(n + 3)  # x values
y = np.zeros(n + 3)  # y values

# Initial conditions
x[0] = 0
x[1] = x[0] + h
x[2] = x[1] + h
c1 = 0  # Example initial condition
y[0] = c1
y[1] = c1 + h * (1 + c1)
y[2] = y[1] + h * (y[1] + np.cos(h))

# Iteratively calculate y and x
for i in range(n):
    y[i + 3] = (-1 / (a * d)) * ((a * e + b * d) * y[i + 2] + (c * d + b * e) * y[i + 1] + c * e * y[i])
    x[i + 3] = x[i + 2] + h

# Load the shared object file for the second code
lib = ctypes.CDLL('./derivative.so')

# Call the main function in the shared library to generate values.dat
lib.main()

# Create a mesh grid for the contour plot
xx = np.linspace(0, 3, 30)
yy = ((math.e ** xx) - np.cos(xx) + np.sin(xx)) / 2

# Read the generated data file
data = np.loadtxt("values.dat", skiprows=1)  # Skip header
x1, y1 = data[:, 0], data[:, 1]

# Plotting
plt.figure(figsize=(10, 6))

# Plot the computed values from the first code
plt.plot(x, y, label="Sim1", color="blue", linewidth=1.5)

# Plot the theoretical contour and simulation data from the second code
plt.plot(xx, yy, color='red', linestyle='dashed', label="Theory")
plt.scatter(x1, y1, s=150, c='orange', label="Sim2", edgecolor='black', linewidth=0.5)

# Formatting the plot
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.savefig('Fig1.png')
plt.show()

