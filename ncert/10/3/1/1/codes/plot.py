import ctypes
import numpy as np
import matplotlib.pyplot as plt
gen = ctypes.CDLL('./points.so')

gen.gen_line_points.argtypes = [
        ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.POINTER(ctypes.c_double)
]

gen.gen_line_points.restype = None

slope_0 = float(1)/7
num_points = 1000
c = 6
h = 0.01
points = np.zeros((num_points, 2), dtype=np.double)

gen.gen_line_points(
    slope_0, c, num_points, 
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
)

fig = plt.figure()
ax = plt.gca()
plt.plot(points[:, 0], points[:, 1], c='r', label = "$x - 7y = -42$")
c = -2
slope_0 = float(1)/3
points = np.zeros((num_points, 2), dtype=np.double)
gen.gen_line_points(
    slope_0, c, num_points, 
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
)
plt.plot(points[:, 0], points[:, 1], c='b',label = "$x - 3y = 6$")
plt.scatter(42, 12, color = "g")
plt.text(42, 10, "(42, 12)", ha="center")
ax.set_xlim(10, 60)
ax.set_ylim(0, 20)
plt.legend(loc = "best")
gen.free_ptr(points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.savefig('../figs/fig.png')

