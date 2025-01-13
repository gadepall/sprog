import ctypes
import numpy as np
import matplotlib.pyplot as plt

lib = ctypes.CDLL('./code.so')
lib.Area.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.Area.restype = ctypes.c_double

a = -1.0 # Lower limit
b = 1.0 # Upper limit
n = 1000 # Number of subintervals

print("Area bounded is: ",lib.Area(a, b, n))

x=np.linspace(-1, 1, 500)
y=x * np.abs(x)

plt.plot(x, y, label='f(x) = x|x|', color='blue')
plt.fill_between(x, y, color='lightblue', alpha=0.5, label='Area Under Curve')
plt.xlabel('x-axis')
plt.ylabel('y-axis')
plt.legend()
plt.grid(True)
plt.show()
