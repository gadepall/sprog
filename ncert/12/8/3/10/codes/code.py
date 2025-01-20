import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#linking the .so file to this python file
dll = ctypes.CDLL('./funcs.so')

#setting the argument types and the return type for the functions in the c code
dll.pointsGenerateLine.argtypes = [ctypes.c_float]*3 + [ctypes.c_int]
dll.pointsGenerateLine.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.pointsGenerateParabola.argtypes = [ctypes.c_float]*3 + [ctypes.c_int]
dll.pointsGenerateParabola.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.area.argtypes = [ctypes.c_float]*2 + [ctypes.c_int]
dll.area.restype = ctypes.c_float

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.freeMemory.restype = None

#setting the initial conditions
x_p = 2
y_p = 4
h_p = 0.0001
n_p = 200000

x_l = 10
y_l = 12
h_l = 0.0001
n_l = 200000

#storing the array of simulated coordinates in the 2D array 'points'
pointsLine = dll.pointsGenerateLine(x_l, y_l, h_l, n_l)
pointsParabola = dll.pointsGenerateParabola(x_p, y_p, h_p, n_p)

coordinatesLine = []
coordinatesParabola = []

#calculating the area enclosed between the parabola and the line
area = dll.area(-2.0, 4.0, n_l)
print("The area enclosed between the parabola and the line is:\n", area)

#storing the coordinates in the block array coordinates
for point in pointsLine[:n_l]:
    coordinatesLine.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinatesLine = np.block(coordinatesLine)

for point in pointsParabola[:n_p]:
    coordinatesParabola.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinatesParabola = np.block(coordinatesParabola)

#plotting the simulated points
plt.scatter(coordinatesLine[0,:], coordinatesLine[1,:], label="Line: y = x + 2", color="blue", linewidths = 1)
plt.scatter(coordinatesParabola[0,:], coordinatesParabola[1,:], label="Parabola: y = $x^2$", color="yellow", linewidths = 1)

#freeing the used memory
dll.freeMemory(pointsLine, n_l)
dll.freeMemory(pointsParabola, n_p)


plt.fill_between(coordinatesLine[0,:], coordinatesLine[1,:], coordinatesParabola[1,:], where=coordinatesLine[0,:] > -2)
plt.fill_between(coordinatesLine[0,:], coordinatesLine[1,:], coordinatesParabola[1,:], where=coordinatesLine[0,:] > 4.05, color = "white")
plt.text(-2, 3, "(-2,3)", fontsize = 20)
plt.text(4, 12, "(4,12)", fontsize = 20)
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-10,10)
plt.ylim(-15,50)
plt.grid(True)
plt.legend()
plt.savefig("../figs/fig.png")
plt.show()
