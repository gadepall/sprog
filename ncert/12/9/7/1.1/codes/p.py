import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'diffEqPoints' and 'freeMultiMem' in the dll
dll.diffEqPoints.argtypes = [ctypes.c_int] + [ctypes.c_float]*4
dll.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 200  # number of points to plot for given differential equation plot
h = 0.01 # step size

# initial conditions, y1 = y, y2 = dy/dx
x = 1.0    # x must be greater than 0 for log(x)
y1 = 0.01  # initial y1
y2 = 1.0   # initial dy/dx

# setting up the plot
plt.figure(figsize=(8, 6))

# getting an array of all the points in the plot
pts = dll.diffEqPoints(n, h, x, y1, y2)

# plotting the differential equation using plt.scatter
coords = []
for i in range(n):
    pt = pts[i]  # access individual point from the pointer
    print(pt[0], pt[1])  # print coordinates to the console for verification
    coords.append(np.array([pt[0], pt[1]]))

coords_plot = np.array(coords)
plt.scatter(coords_plot[:, 0], coords_plot[:, 1], marker=".", label="Sim", color="royalblue")

# freeing the memory of the array 'pts'
dll.freeMultiMem(pts, n)

# customize the plot (axes, labels, grid)
ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

plt.legend(loc='best')
plt.grid()
plt.axis('equal')

plt.xlabel('x')
plt.ylabel('y')

# Display the plot
plt.show()

