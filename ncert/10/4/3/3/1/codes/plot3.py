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
st1=4
st2=-0.5
eps=0.0000001
#Calling C Function

stef=ctypes.CDLL('./steffensen.so')
stef.steff1.argtypes=[ctypes.c_double]
stef.steff1.restype=ctypes.c_double

stef.steff2.argtypes=[ctypes.c_double]
stef.steff2.restype=ctypes.c_double
#Calculating first root
max_iterations = 1000  # Define a reasonable limit
iteration = 0
flag=1

while flag == 1 and iteration < max_iterations:
    newst1 = stef.steff1(ctypes.c_double(st1))
    if abs(newst1 - st1) > eps:
        st1 = newst1
        flag = 1
    else:
        flag = 0
    iteration += 1

if iteration >= max_iterations:
    print("Warning: Maximum iterations reached for st1.")

flag =1
while flag == 1 and iteration < max_iterations:
    newst2 = stef.steff2(ctypes.c_double(st2))
    if abs(newst2 - st2) > eps:
        st2 = newst2
        flag = 1
    else:
        flag = 0
    iteration += 1

if iteration >= max_iterations:
    print("Warning: Maximum iterations reached for st2.")





print(st1,st2)
# Plotting the function
plt.plot(x, y, label='theory', color='orange')
plt.plot(st1,f(st1),'ro',label='(3.3027756563007706,0)')
plt.plot(st2,f(st2),'bo',label='(-0.302775637731847,0)')
# Add labels and title
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Plot of the quadratic equation: $f(x) = x^2 - 3x - 1$')

# Add a grid
plt.grid(True)

# Show the legend
plt.legend()
plt.savefig("steffenson.jpeg")
plt.show()

