import ctypes
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.lines import Line2D  

data = np.loadtxt("values.dat", skiprows=1)  # Skip header
x, y = data[:, 0], data[:, 1]  # Separate x and y columns into arrays
prob = y/x
xx = np.linspace(0,22408,50)
yy =xx*0 + 1/6
# Plot the contour and scatter points
plt.figure(figsize=(8, 8))
plt.plot(xx,yy, c = 'red', label = "Actual")
plt.scatter(x, prob, s=1, c='blue', label="Sim")
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.grid(color='gray', linestyle='--', linewidth=0.5)
plt.xlabel('Number of Iterations')
plt.ylabel('Probability')
plt.legend()
plt.savefig('../figs/sim.png')
plt.show()

