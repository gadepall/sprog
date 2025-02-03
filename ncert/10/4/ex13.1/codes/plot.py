import ctypes
import matplotlib.pyplot as plt
import math


lib = ctypes.CDLL('./lib.so')
xo = 5/6
#Pointer types and pointers to be used 
FUNCPTR = ctypes.CFUNCTYPE(ctypes.c_double)
deriv = FUNCPTR(("deriv", lib))
func = FUNCPTR(("func", lib))
#Argument types for the functions in C
lib.makePlotArray.argtypes = [
    ctypes.c_double,                
    ctypes.c_double,                
    ctypes.c_int,                  
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_double,
    FUNCPTR
    ]
lib.NewtonRootFind.argtypes = [
    ctypes.c_double,
    ctypes.c_int,
    FUNCPTR,
    FUNCPTR,
    ctypes.POINTER(ctypes.c_double)
    ]
#Variables for the point generation function
n = 10000
x = (ctypes.c_double * n)()
y = (ctypes.c_double * n)()
lib.makePlotArray(0 ,2, n, x, y, 0.01, func)
#Formatting the graph and plotting the points
ax = plt.gca()
ax.set_xlim([0, 2])
ax.set_ylim([-0.5, 1])
plt.grid()
plt.plot(x,y, label = 'theory')
#Variables for the Newton-Raphson function and then running, and plotting
n = 200
root = (ctypes.c_double * 1)()
lib.NewtonRootFind(-10, n, func, deriv, root)
plt.plot(root, lib.func(root),'o', label='Numerically found root')
lib.NewtonRootFind(10, n, func, deriv, root)
plt.plot(root, lib.func(root),'o', label='Numerically found root')

plt.legend()
plt.savefig("/home/gvt1/sdcard/github/EE1003/Assignment4/figs/fig.png")
plt.show()
