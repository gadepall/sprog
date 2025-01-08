import numpy as np
import ctypes
import matplotlib.pyplot as plt

gen = ctypes.CDLL('./points.so')

class Points(ctypes.Structure):
    _fields_ = [("t", ctypes.POINTER(ctypes.c_double)),
                ("P", ctypes.POINTER(ctypes.c_double))]

gen.generate_points.argtypes = [ctypes.POINTER(Points), ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
gen.generate_points.restype = None

gen.free_points.argtypes = [ctypes.POINTER(Points)]
gen.free_points.restype = None

P_0 = 100.0
t_0 = 0.0
t_end = 10.0
h = 0.1

points = Points()

gen.generate_points(ctypes.byref(points), P_0, t_0, t_end, h)

size = int((t_end - t_0) / h) + 1

t_sim = np.ctypeslib.as_array(points.t, shape=(size,))
P_sim = np.ctypeslib.as_array(points.P, shape=(size,))

plt.figure()
plt.plot(t_sim, P_sim, c='r', marker='.', label="sim")

t_theory = np.linspace(t_0, t_end, 1000)
P_theory = P_0 * np.exp(t_theory * 6.931 / 100)
plt.plot(t_theory, P_theory, c='g', label="theory")

plt.xlabel("t")
plt.ylabel("P")
plt.legend()
plt.savefig('../figs/fig.png')
plt.show()

gen.free_points(ctypes.byref(points))

