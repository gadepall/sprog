import numpy as np
import matplotlib.pyplot as plt
import ctypes

#linking the .so file to this python file
dll = ctypes.CDLL('./ccode.so')

#setting the argument types and the return type for the functions in the c code
dll.pointsGenerate.argtypes = [ctypes.c_float]*3 + [ctypes.c_int]
dll.pointsGenerate.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.freeMemory.restype = None

#setting the initial conditions
x = 0
y = 0
h = 0.01
n = 1000

#storing the array of simulated coordinates in the 2D array 'points'
points = dll.pointsGenerate(x, y, h, n)

coordinates = []

#storing the coordinates in the block array coordinates
for point in points[:int(3 * (n/2))]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#plotting the simulated points
scatter = plt.scatter(coordinates[0,:], coordinates[1,:], label="Sim", color="red")

#freeing the used memory
dll.freeMemory(points, n)

#plotting the actual implicit expression using contour function
xrange = np.arange(-5,10,h)
yrange = np.arange(-5,18,h)
x, y = np.meshgrid(xrange, yrange)
function = 3*x - 6*y + (9/7)*(np.log(np.abs((28*x + 42*y + 44)/44)))
contour = plt.contour(x, y, function, [0], colors = 'black', linewidths = 2)

#creating a proxy line to be able to label the contour graph
proxy_line = plt.Line2D([0], [0], color='black', linestyle='-', label='Theory')

plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-6,11)
plt.grid(True)
plt.legend(handles=[scatter, proxy_line], loc='upper right')
plt.savefig("../figs/simulated.png")
plt.show()
