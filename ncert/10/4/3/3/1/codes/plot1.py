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
nr1=4
nr2=-0.5
eps=0.0000001
#Calling C Function

newton=ctypes.CDLL('./nr.so')
newton.nr.argtypes=[ctypes.c_double]
newton.nr.restype=ctypes.c_double
#Calculating first root
flag=1
while flag==1:
    newnr1 = newton.nr(ctypes.c_double(nr1))
    if abs(newnr1-nr1) > eps:
        nr1=newnr1
        flag=1
    else:
        flag=0


#Calculating second root
flag=1

while flag==1:
    newnr2 = newton.nr(ctypes.c_double(nr2))    
    if abs(nr2-newnr2)>eps:
        nr2=newnr2
        flag=1
    else:
        flag=0




print(nr1,nr2)
# Plotting the function
plt.plot(x, y, label='theory', color='blue')
plt.plot(nr1,f(nr1),'go',label='(3.3027756377328092,0)')
plt.plot(nr2,f(nr2),'ro',label='(-0.30277563792072887,0)')
# Add labels and title
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Plot of the quadratic equation: $f(x) = x^2 - 3x - 1$')

# Add a grid
plt.grid(True)

# Show the legend
plt.legend()
plt.savefig("Newton-Raphson.jpeg")
plt.show()

