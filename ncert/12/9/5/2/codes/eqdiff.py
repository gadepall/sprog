#Code by S.Sai Akshita
#importing necessary libraries

import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#Defining variables(selecting random point on the function)

x=[0]
y=[2]
h=0.01

#Calling C function
function=ctypes.CDLL('./generate.so')
function.coordinate.argtypes=[ctypes.c_double,ctypes.c_double]
function.coordinate.restype=ctypes.c_double


#Generating points on the graph using for loop
for i in range(0,1000):
    x.append(x[i]+h)
    y.append(function.coordinate(x[i],y[i]))
#end of for loop
x=np.array(x)
y=np.array(y)

#Defining the general solution function
z=np.exp(-2 * x) + np.exp(-3 * x)
#Plotting the function graph

plt.plot(x,y,linewidth=8,label="Sim",color='pink')
plt.plot(x,z,linewidth=2,label="Theory ",color='red')
plt.legend()
plt.xlabel('X-coordinates')
plt.ylabel('Y-coordinates')
plt.grid(True)
plt.savefig('Graph.jpeg')
