import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#linking the .so file to this python file
dll = ctypes.CDLL('./ccode.so')

#setting the argument types and the return type for the functions in the c code
dll.pointsGenerate.argtypes = [ctypes.c_float]*3 + [ctypes.c_int]
dll.pointsGenerate.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.newtonRalphson.argtypes = [ctypes.c_float] + [ctypes.c_int]
dll.newtonRalphson.restype = ctypes.c_float

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.freeMemory.restype = None

#setting the initial conditions
x = -60
y = 1500
h = 0.001
n = 150000

#roots of the quadratic equation through Newton Ralphson method
print("Root 1: ", dll.newtonRalphson(100, n))
print("Root 2: ", dll.newtonRalphson(-100, n))

#storing the array of simulated coordinates in the 2D array 'points'
points = dll.pointsGenerate(x, y, h, n)

coordinates = []

#storing the coordinates in the block array coordinates
for point in points[:n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#plotting the simulated points
plt.plot(coordinates[0,:], coordinates[1,:], label="Sim", color="red")
plt.scatter(dll.newtonRalphson(100, n), 0, color = "green", label = "Root-1", linewidths = 2)
plt.scatter(dll.newtonRalphson(-100, n), 0, color = "blue", label = "Root-2", linewidths = 2)
plt.text(dll.newtonRalphson(100, n), 0, "(40, 0)", color = "black", fontsize = 15)
plt.text(dll.newtonRalphson(-100, n), 0, "(-45, 0)", color = "black", fontsize = 15)

#freeing the used memory
dll.freeMemory(points, n)


plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-60,80)
plt.ylim(-2000,5000)
plt.grid(True)
plt.legend()
plt.savefig("../figs/simulated.png")
plt.show()
