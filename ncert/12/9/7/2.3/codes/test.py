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
h = 0.01

def double_derivative(x,y):
    return  6*np.cos(3*x)-9*y # returns the differential equation

# Arrays to store x and y values
x = np.empty(100, dtype=float)
y = np.empty(100, dtype=float)

solve = CDLL('./test.so')  # loads all the functions that are in test.c
solve.generate_xpoints.restype = c_double  # return datatype of generate_xpoints
solve.generate_ypoints.restype = c_double  # return datatype of generate_ypoints

x[0], y[0] = x0, y0  # initial conditions

# by using taylor series

y[2] = y[0] + 0.5*double_derivative(x[0],y[0])*np.square(h)
y[1] = double_derivative(x[0],y[0])*np.square(h) + 2*y[0] - y[2]
x[1] = x[0]-h
x[2] = h 

# Solve for x and y coordinates

for i in range(3, 100):
    dybydx = double_derivative(x[i-1],y[i-1])
    x[i] = solve.generate_xpoints(c_double(x[i-1]), c_double(h))  # function call to get x coordinates
    y[i] = solve.generate_ypoints(c_double(y[i-2]), c_double(y[i-1]),c_double(dybydx),c_double(h))  # function call to get y coordinates

# Plotting the simulation graph
plt.plot(x, y, color='yellow', label='Simulation Graph',linewidth=8)

# Plotting the theoretical graph
x_theoretical = x 
y_theoretical =  x*np.sin(3*x)
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

