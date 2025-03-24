#Code by S.Sai Akshita
#importing libraries
import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math


# Define the quadratic function
def f(x):
    return x**2 - 3*x - 1
#Defining variables
x = np.linspace(-5, 5, 400)
y = f(x) #y values theoretically
fpi1=4
fpi2=-1.5
eps=0.00001
#Calling C Functions

point1=ctypes.CDLL('./fip.so')
point1.fip1.argtypes=[ctypes.c_double]
point1.fip1.restype=ctypes.c_double

point2=ctypes.CDLL('./fip.so')
point2.fip2.argtypes=[ctypes.c_double]
point2.fip2.restype=ctypes.c_double


#Calculating first root
flag=1
while flag==1:
  newfpi1 = point1.fip1(ctypes.c_double(fpi1))
  if abs(newfpi1-fpi1) > eps:
        fpi1=newfpi1
        flag=1
  else:
        flag=0


#Calculating second root
flag=1

while flag==1:
    newfpi2 = point2.fip2(ctypes.c_double(fpi2))    
    if abs(fpi2-newfpi2)>eps:
        fpi2=newfpi2
        flag=1
    else:
        flag=0




print(fpi1,fpi2)
# Plotting the function
plt.plot(x, y, label='theory', color='green')
plt.plot(fpi1,f(fpi1),'o',color='red',label='(3.302771855010661,0)')
plt.plot(fpi2,f(fpi2),'o',color='yellow',label='(-0.30277386101541914,0)')
# Add labels and title
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Plot of the quadratic equation: $f(x) = x^2 - 3x - 1$')

# Add a grid
plt.grid(True)

# Show the legend
plt.legend()
plt.savefig("fpi.jpeg")
plt.show()

