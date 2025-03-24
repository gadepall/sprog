import matplotlib.pyplot as plt
import numpy as np
from ctypes import *

# Load the shared object file
func = CDLL('./code.so')

# Assign C functions in Python
x_n = func.x_n
y_n = func.y_n

# Set return data types for C functions
x_n.restype = c_double
y_n.restype = c_double

# Initialize values
x_0 = 1
y_0 = 0
h = 0.003

# Theoretical plot
x = np.linspace(0, 1, 1000)
y = np.sqrt(1-(x*x))
plt.plot(x, y, label="Theory", linewidth=2, color='blue')

# Simulated points using the finite difference method
x_sim = [x_0]
y_sim = [y_0]

for i in range(332): 
    #x_0 = x_n(c_double(x_0), c_double(h))
    #y_0 = y_n(c_double(x_0), c_double(y_0), c_double(h))
    y_0 = y_n(c_double(y_0), c_double(h))
    x_0 = x_n(c_double(x_0), c_double(y_0), c_double(h))
    x_sim.append(x_0)
    y_sim.append(y_0)

plt.plot(x_sim, y_sim, 'ko', markersize=2, label="Simulated")

# Labels, grid, and legend
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()

# Save and show the plot
plt.savefig('fig.pdf')
plt.show()
