import ctypes
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.lines import Line2D  # For creating custom legend handles

# Load the shared object file
lib = ctypes.CDLL('./derivative.so')

# Call the main function in the shared library to generate values.dat
lib.main()

# Read the generated data file
data = np.loadtxt("values.dat", skiprows=1)  # Skip header
x, y = data[:, 0], data[:, 1]  # Separate x and y columns into arrays

# Create a mesh grid for the contour plot
delta = 0.025
xx = np.arange(-40, 40, delta)
yy = np.arange(-40, 40, delta)
x1, y1 = np.meshgrid(xx, yy)

# Define the equation for the contour plot
eq = x1 * y1 * np.abs(np.cos(y1 / x1)) - 0.54

# Plot the contour and scatter points
plt.figure(figsize=(8, 8))
C = plt.contour(x1, y1, eq, [0], colors='black', linestyles=['dashed'])  # Contour plot for the equation
plt.scatter(x, y, s=1, c='yellow', label="Sim")
plt.scatter(-x, -y, s=1, c='yellow')  # Adding reflected points
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.xlabel('x')
plt.ylabel('y')
# Create custom legend handles
simulation_proxy = Line2D([0], [0], marker='o', color='w', markerfacecolor='yellow', markersize=5, label='Sim')
theoretical_proxy = Line2D([0], [0], color='black', linestyle='dashed', label='Theory')

# Add legend with proxy handles
plt.legend(handles=[simulation_proxy, theoretical_proxy])
plt.savefig('fig1.png')
plt.show()

