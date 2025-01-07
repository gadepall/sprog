import ctypes
import numpy as np
import matplotlib.pyplot as plt
from math import sin, exp, cos
gen = ctypes.CDLL('./points.so')

gen.generate_points.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_double, 
    ctypes.POINTER(ctypes.c_double), ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double) , ctypes.c_int
]

gen.generate_points.restype = None

x_0, y_0 = 0, 1
num_points = 300
h = 0.01
points = np.zeros((num_points, 2), dtype=np.double)
def my_sin(x):
    return sin(x)
pass_func = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(my_sin)
gen.generate_points(
    x_0, y_0, h, 
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), pass_func, 
    num_points
)
fig = plt.figure()
ax = plt.gca()
plt.plot(points[:, 0], points[:, 1], c='r',marker = "o", label = "sim2")
x = np.linspace(0.0, 10, 1000)
y = 1.2*np.exp(-2*x) + 0.4*np.sin(x) - 0.2*np.cos(x)
plt.plot(x, y, c='g', label = "theory")
points = np.zeros((int(num_points/10), 2), dtype=np.double)
gen.generate_points(
    x_0, y_0, h * 10, 
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), pass_func,
    int(num_points/10)
)
plt.plot(points[:, 0], points[:, 1], c='b', marker = "o", label = "sim1")
ax.set_xlim(0, 3)
ax.set_ylim(0, 3)
plt.legend(loc = "best")
gen.free_ptr(points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.savefig('../figs/fig.png')
plt.show()
