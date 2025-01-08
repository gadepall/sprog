

import numpy as np
import matplotlib.pyplot as plt
import os
from ctypes import *

# Get the directory where the current code is located
script_dir = os.path.dirname(os.path.abspath(__file__))
# path we want to save this figure 
save_directory = os.path.join(script_dir, "../fig")

if not os.path.exists(save_directory):
    os.makedirs(save_directory)  # creates directory if not exists

# Initial conditions
x0, y0 = 0.0, 0.0
h = 0.1

def derivative(x,y):
    return (2*np.exp(-y)-1)/(x+1)  # returns the differential equation

# Arrays to store x and y values
x = np.empty(100, dtype=float)
y = np.empty(100, dtype=float)

solve = CDLL('./test.so')  # loads all the functions that are in test.c
solve.generate_xpoints.restype = c_double  # return datatype of generate_xpoints
solve.generate_ypoints.restype = c_double  # return datatype of generate_ypoints

x[0], y[0] = x0, y0  # initial conditions

# Solve for x and y coordinates
for i in range(1, 100):
    dybydx = derivative(x[i-1],y[i-1])
    x[i] = solve.generate_xpoints(c_double(x[i-1]), c_double(h))  # function call to get x coordinates
    y[i] = solve.generate_ypoints(c_double(y[i-1]), c_double(h),c_double(dybydx))  # function call to get y coordinates

# Plotting the simulation graph
plt.plot(x, y, color='yellow', label='Simulation Graph',linewidth=8)

# Plotting the theoretical graph
x_theoretical = x
y_theoretical = np.log((2*x+1)/(x+1))
plt.plot(x_theoretical, y_theoretical, color='red', label='Theoretical Graph',linewidth=1)

# Final plot adjustments
plt.grid()
plt.axis("equal")
plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.legend()

# Save the combined figure
save_path = os.path.join(save_directory, 'combined_fig.jpg')  # saves as combined_fig.jpg
plt.savefig(save_path)
plt.show()
