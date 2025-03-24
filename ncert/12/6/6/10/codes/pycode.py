import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math
import cvxpy as cp

#linking the .so file to this python file
dll = ctypes.CDLL('./ccode.so')

#setting the argument types and the return type for the functions in the c code
dll.pointsGenerate.argtypes = [ctypes.c_float]*4 + [ctypes.c_int]
dll.pointsGenerate.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.minima.argtypes = [ctypes.c_float]*4 + [ctypes.c_int]
dll.minima.restype = ctypes.c_float

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.freeMemory.restype = None

#setting the initial conditions
y = 156.25 * math.pi**2
x = -10
h = 0.001
k = 10 * math.pi
tol = 1e-6
n = 200000

locMinima = dll.minima(x, h, tol, k, n)
print("Area is minimum when radius of circle is: ", locMinima)

#storing the array of simulated coordinates in the 2D array 'points'
points = dll.pointsGenerate(x, y, h, k, n)

coordinates = []

#storing the coordinates in the block array coordinates
for point in points[:n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Solving using geometric programming

#Define the variables
x1 = cp.Variable(pos=True)

#Defining the objective function
objective = cp.Minimize((0.25 * (math.pi**2 + (4 * math.pi)) * (x1**2)) - (0.25 * math.pi * k * x1) + ((k**2) * 0.0625))

#Defining the problem
problem = cp.Problem(objective)

#Solving the problem
problem.solve(gp=True)

#plotting the simulated points
plt.plot(coordinates[0,:], coordinates[1,:], label="Objective function", color="red")

#plotting the local minima point
plt.scatter(locMinima, (0.25 * (math.pi**2 + (4 * math.pi)) * (locMinima**2)) - (0.25 * math.pi * k * locMinima) + ((k**2) * 0.0625), label = "point of minima", linewidths = 2)

#pllotting the local minima point obtained through geometric programming
plt.scatter(x1.value , problem.value, color = 'black', label='Geometric Programming', linewidths = 1)

#freeing the used memory
dll.freeMemory(points, n)


plt.xlabel("r")
plt.ylabel("A")
plt.xlim(-10,10)
plt.ylim(-100,2000)
plt.grid(True)
plt.legend()
plt.savefig("../figs/simulated.png")
plt.show()
