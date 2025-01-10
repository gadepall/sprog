import ctypes
import matplotlib.pyplot as plt
import numpy as np
import math

# Load the shared object file
lib = ctypes.CDLL('./derivative.so')

# Call the main function in the shared library to generate values.dat
lib.main()

# Create a mesh grid for the contour plot
xx = np.linspace(0, 3, 30)
yy = ((math.e)**xx) * np.cos(xx)

# Plot the contour and scatter points
plt.figure(figsize=(8, 8))

# Read the generated data file
data = np.loadtxt("values.dat", skiprows=1)  # Skip header
x, y = data[:, 2], data[:, 3]  # Separate x and y columns into arrays
x1, y1 = data[:, 0], data[:, 1]

# Results are stored in y and y_prime
plt.plot(xx, yy, color='red', linestyle='dashed', label="Theory")  # Contour plot for the equation
plt.scatter(x, y, s=30, c='blue', label="Sim2")  # Make Sim2 more visible
plt.scatter(x1, y1, s=20, c='orange', label="Sim1", edgecolor='black', linewidth=0.5)  # Highlight Sim1
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.xlabel('x')
plt.ylabel('y')

# Add legend with proxy handles
plt.legend()
plt.savefig('fig1.png')
plt.show()

