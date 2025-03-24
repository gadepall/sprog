import ctypes
import numpy as np
import matplotlib.pyplot as plt
gen = ctypes.CDLL('./points.so')

gen.generate_points.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, 
    ctypes.c_double, ctypes.c_double,
    ctypes.POINTER(ctypes.c_double), ctypes.c_int
]

gen.generate_points.restype = None

x_0, y_0, a, k = 0, 1, 5, 0
slope_0 = -5
num_points = 100
h = 0.01
points = np.zeros((num_points, 2), dtype=np.double)

gen.generate_points(
    a, k, x_0, y_0, slope_0, h, 
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

fig = plt.figure()
ax = plt.gca()
plt.plot(points[:, 0], points[:, 1], c='r',marker = "o", label = "sim2")
x = np.linspace(0.0, 10, 1000)
y = np.exp(-5*x)
plt.plot(x, y, c='g', label = "theory")
points = np.zeros((int(num_points/10), 2), dtype=np.double)
gen.generate_points(
    a, k, x_0, y_0, slope_0, h * 10,
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    int(num_points/10)
)
plt.plot(points[:, 0], points[:, 1], c='b', marker = "o", label = "sim1")
'''
ax.text(x1, y1, ' (5,4)', color='red', fontsize=12)
ax.text(x2, y2, ' (9,-2)', color='green', fontsize=12)
ax.text(x3, y3, ' (7,1)', color='blue', fontsize=12)
'''
ax.set_xlim(0, num_points * h)
plt.legend(loc = "best")
gen.free_ptr(points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.savefig('../figs/fig.png')

