import ctypes
import matplotlib.pyplot as plt
import numpy as np

lib = ctypes.CDLL('./10.4.3.11.so')
lib.coords.restype = ctypes.POINTER(ctypes.c_double)
lib.newton.restype = ctypes.c_double
lib.bisection.restype = ctypes.c_double

result = lib.coords()
rootn = lib.newton()
rootb= lib.bisection()
yn = rootn*rootn - 6*rootn - 216;
yb = rootb*rootb - 6*rootb - 216;

xcoords = list()
ycoords = list()
for i in range(0,999,2):
        xcoords.append(result[i])
        ycoords.append(result[i+1])
plt.scatter(18,0,color='k',s=600,label="theory")
plt.scatter(rootn,yn,color='m',s=400,label="sim1(newton)")
plt.text(rootn,yn,f'({round(rootn,2)},{round(yn,2)})',fontsize=15,ha='right',va = 'top')
plt.scatter(rootb,yb,color='g',s=200,label="sim2(bisection)")
plt.plot(xcoords,ycoords,color='r',label="functional curve",linewidth = 5)

print("value of sides of squares by newton raphson method are:", rootn,"and",rootn - 6)
print("value of sides of squares by bisection method are:", rootb,"and",rootb - 6)
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
# To get a grid
plt.grid(True)
plt.show()
