import ctypes
import matplotlib.pyplot as plt
import math


lib = ctypes.CDLL('./lib.so')
yo=1.0
xo=5.0

FUNCPTR = ctypes.CFUNCTYPE(ctypes.c_double)
deriv = FUNCPTR(("deriv", lib))
func = FUNCPTR(("func", lib))

lib.makePlotArray.argtypes = [
    ctypes.c_double,                
    ctypes.c_double,                
    ctypes.c_int,                  
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_double
    ]
lib.gradDesc.argtypes = [
    ctypes.c_double,               
    ctypes.c_double,   
    ctypes.c_double,
    ctypes.POINTER(ctypes.c_double),
    FUNCPTR,
    FUNCPTR
    ]
n = 1000000
x = (ctypes.c_double * n)()
y = (ctypes.c_double * n)()
lib.makePlotArray(0, 16, n, x, y, 0.00001)
#plt.axis('equal')
ax = plt.gca()
ax.set_xlim([-2, 4])
ax.set_ylim([-1, 20])
plt.grid()
plt.plot(x,y, label = 'theory')
coords=(ctypes.c_double *2)()
lib.gradDesc(3, -0.00001, 1e-9, coords, func, deriv)
plt.plot(coords[0], coords[1],'o',label='minima')
lib.gradDesc(-2, +0.00001, 1e-9, coords, func, deriv)
plt.plot(coords[0], coords[1],'o',label='inflection')
lib.gradDesc(0.29, 0.00001, 1e-9, coords, func, deriv)
plt.plot(coords[0], coords[1],'o',label='maxima')
plt.legend()
plt.savefig("/home/gvt1/sdcard/github/EE1003/Assignment3/figs/fig.png")
plt.show()
