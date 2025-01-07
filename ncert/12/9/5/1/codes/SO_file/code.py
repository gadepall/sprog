import ctypes
import numpy as np
import matplotlib.pyplot as plt

funcs = ctypes.CDLL('./funcs.so')

dif = funcs.dif
dif.restype = ctypes.c_double

theoretical_function = funcs.theoretical_function
theoretical_function.restype = ctypes.c_double

resolution = 20
x1 = 0
x2 = 5

x = np.linspace(x1, x2, resolution)
h = (x2 - x1) / resolution

y = np.zeros(resolution)

for i in range(1, resolution):
    y[i] = y[i-1] + dif(ctypes.c_double(x[i-1]), ctypes.c_double(y[i-1])) * h

y_theoretical = np.zeros(resolution)

for i in range(resolution):
    y_theoretical[i] = theoretical_function(ctypes.c_double(x[i]))

plt.plot(x, y_theoretical, label = 'Theory', color = 'red')
plt.scatter(x, y, label = 'Sim')

ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

plt.axis('equal')
plt.grid()
plt.legend(loc='best')

plt.savefig('../../figs/fig1_.png')
# plt.show()
