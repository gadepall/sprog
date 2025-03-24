import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#linking the .so file to this python file
dll = ctypes.CDLL('./ccode.so')

#setting the argument types and the return type for the functions in the c code
dll.pointsGenerate.argtypes = [ctypes.c_float]*3 + [ctypes.c_int]
dll.pointsGenerate.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.freeMemory.restype = None

#setting the initial conditions
x = -7.58516
y = -114.23739
h = 0.001
n = 200000

#storing the array of simulated coordinates in the 2D array 'points'
points = dll.pointsGenerate(x, y, h, n)

coordinates = []

#storing the coordinates in the block array coordinates
for point in points[:n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#plotting the simulated points
plt.scatter(coordinates[0,:], coordinates[1,:], label="Sim", color="red", linewidths = 3)

#freeing the used memory
dll.freeMemory(points, n)

#plotting the actual points
x_coord = []
y_coord = []
x = -7.58516
for i in range(n):
    y = (((2 * x**2)/math.sin(x)) - (math.pi**2/(2 * math.sin(x))))
    x_coord.append(x)
    y_coord.append(y)
    x += h
plt.scatter(x_coord, y_coord, label = "Theory", color = "blue", linewidths = 0.1)

plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-7.5,10)
plt.ylim(-150,150)
plt.grid(True)
plt.legend()
plt.savefig("../figs/simulated.png")
plt.show()
