import numpy as np

import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./LU.so')

#specifying argument, and return types for all functions defined in the C program

dll.LUsolver.argtypes = [ctypes.c_int]
dll.LUsolver.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))


# Plotting the lines

x = np.linspace(-5, 5, 1000)
y1 = (1.3 - 0.2*x)/0.3
y2 = (2.3 - 0.4*x)/0.5

plt.plot(x, y1, color='r', label = "0.2x + 0.y = 1.3")
plt.plot(x, y2, color = 'b', label = '0.4x + 0.5y = 2.3')


# Verifying with LU decomposition
i = dll.LUsolver(2)
plt.scatter(i[0][0], i[1][0], color='g', label='Solution')

#initial conditions

#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()
#plt.xlim(0, 5)
#plt.ylim(0, 50)
#plt.show()
plt.savefig("../plots/plot.png")
