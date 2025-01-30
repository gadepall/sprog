import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#linking the .so file to this python file
dll = ctypes.CDLL('./ccode.so')

#setting the argument types and the return type for the functions in the c code
dll.pointsGenerate.argtypes = [ctypes.c_float]*5 + [ctypes.c_int]
dll.pointsGenerate.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.freeMemory.restype = None

#setting the initial conditions
a_1 = math.sqrt(2)
b_1 = math.sqrt(3)
c_1 = 0
a_2 = math.sqrt(3)
b_2 = -math.sqrt(8)
c_2 = 0
x = -10
h = 0.001
n = 100000

#storing the array of simulated coordinates in the 2D array 'points'
points_1 = dll.pointsGenerate(a_1, b_1, c_1, x, h, n)
points_2 = dll.pointsGenerate(a_2, b_2, c_2, x, h, n)

coordinates_1 = []
coordinates_2 = []

#storing the coordinates in the block array coordinates
for point in points_1[:n]:
    coordinates_1.append(np.array([[point[0], point[1]]]).reshape(-1, 1))
for point in points_2[:n]:
    coordinates_2.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates_1 = np.block(coordinates_1)
coordinates_2 = np.block(coordinates_2)

#plotting the simulated points
plt.plot(coordinates_1[0,:], coordinates_1[1,:], label="\u221A2x + \u221A3y = 0", color="red")
plt.plot(coordinates_2[0,:], coordinates_2[1,:], label="\u221A3x - \u221A8y = 0", color="black")
plt.text(0, 0, "(0,0)", fontsize = 16, horizontalalignment = "center", verticalalignment = "bottom")

#freeing the used memory
dll.freeMemory(points_1, n)
dll.freeMemory(points_2, n)

plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-15,100)
plt.ylim(-100,100)
plt.grid(True)
plt.legend()
plt.savefig("../figs/simulated.png")
plt.show()
