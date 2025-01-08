import ctypes
import matplotlib.pyplot as plt
import math


lib = ctypes.CDLL('./lib.so')
yo=1.0
xo=5.0

DYBYDX = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
dybydx_func = DYBYDX(("dybydx", lib))

lib.finDif.argtypes = [
    ctypes.c_double,                # xo
    ctypes.c_double,                # yo
    ctypes.c_int,                   # n
    ctypes.POINTER(ctypes.c_double), # x (array)
    ctypes.POINTER(ctypes.c_double), # y (array)
    ctypes.c_double,
    DYBYDX# dybydx (function pointer)
    ]
lib.improvedEulers.argtypes = [
    ctypes.c_double,                # xo
    ctypes.c_double,                # yo
    ctypes.c_int,                   # n
    ctypes.POINTER(ctypes.c_double), # x (array)
    ctypes.POINTER(ctypes.c_double), # y (array)
    ctypes.c_double,
    DYBYDX# dybydx (function pointer)
    ]
lib.runOurFunc.argtypes = [
    ctypes.c_double,                # xo
    ctypes.c_double,                # yo
    ctypes.c_int,                   # n
    ctypes.POINTER(ctypes.c_double), # x (array)
    ctypes.POINTER(ctypes.c_double), # y (array)
    ctypes.c_double
    ]

n = 1000000
x = (ctypes.c_double * n)()
y = (ctypes.c_double * n)()
lib.finDif(xo , yo, n, x, y, 0.0001, dybydx_func)
#plt.axis('equal')
ax = plt.gca()
ax.set_xlim([0, 60])
ax.set_ylim([-1, 1.6])
plt.grid()
plt.plot(x,y,'o', label = 'sim (Euler\'s)')
n = 1000000
x = (ctypes.c_double * n)()
y = (ctypes.c_double * n)()
lib.improvedEulers(xo, yo, n, x, y, 0.0001, dybydx_func)
plt.plot(x,y,'o', label = 'sim (Improved Euler\'s)')
n = 10000000
x = (ctypes.c_double * n)()
y = (ctypes.c_double * n)()
y[0] = 1
x[0] = 5
lib.runOurFunc(x[0], y[0], n, x, y, 0.00001)
plt.plot(x, y, label = 'theory')
plt.legend()
plt.savefig("/home/gvt1/sdcard/github/EE1003/Assignment1/figs/all.png")
#plt.show()
