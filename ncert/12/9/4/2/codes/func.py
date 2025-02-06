import matplotlib.pyplot as plt
import math
import numpy as np
from ctypes import *

# Loading the shared object file 
func = CDLL('./func.so')

# Assigning the c functions in python
x_n = func.x_n
y_n = func.y_n

# Setting the return data types of the C functions
x_n.restype=c_double
y_n.restype=c_double

# Initialising the values of x_0,y_0 and h
x_0=1
y_0=0
h=0.006

# Theoretical plot
x=np.linspace(1,3,500)
y=x*np.log(x)
plt.plot(x, y, label="Theory",linewidth=2, color='blue')

# Simulated points using the finite difference method
for i in range(250):
    x_0=x_n(c_double(x_0),c_double(h))
    y_0=y_n(c_double(x_0),c_double(y_0),c_double(h))
    plt.plot(x_0,y_0,'ko',markersize=2)

# Labels, grid, and legend
plt.plot(x_0,y_0,'ko',label='Sim')
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()

plt.savefig('fig.pdf')
plt.show()
