#Code by S.Sai Akshita
#importing libraries
import numpy as np
import matplotlib.pyplot as plt
import math
import ctypes

#Defining variables for simulation
h=0.1
a=[-3.9999]
b=[0.067]
#Calling C function
function=ctypes.CDLL('./sim.so')
function.smolarea.argtypes=[ctypes.c_double,ctypes.c_double]
function.smolarea.restype=ctypes.c_double



#Generating points for the trapezoids on the graph using for loop
for i in range(0,80):
    a.append(a[i]+h)
    b.append(function.smolarea(a[i],b[i]))

#end of for loop
a=np.array(a)
b=np.array(b)

# Parameters for trapezoidal rule
n = 80  # Number of subintervals
x_trap = a  # x values at the trapezoid boundaries
y_trap = b  # y values at the trapezoid boundaries


#plotting theoretical ellipse
p = 4  # Semi-major axis
q = 3  # Semi-minor axis
# Parameter t from 0 to 2π
t = np.linspace(0, 2 * np.pi, 500)
# Parametric equations for the ellipse
k = p * np.cos(t)
l = q * np.sin(t)
# Plot the ellipse
plt.figure(figsize=(8, 6))
plt.plot(k, l, label="Theory", color='blue')
#Plotting Trapezoids
for i in range(n):
    # Vertices of each trapezoid
    x_trapezoid = [x_trap[i], x_trap[i], x_trap[i + 1], x_trap[i + 1]]
    y_trapezoid = [-1*y_trap[i], y_trap[i], y_trap[i + 1], -1*y_trap[i+1]]
    plt.fill(x_trapezoid, y_trapezoid, color='orange', alpha=0.5, edgecolor='red', linewidth=1)

plt.title("Trapezoidal Approximation of the Ellipse")
plt.xlabel("x")
plt.ylabel("y")
plt.axhline(0, color='black', linewidth=0.8, linestyle="--")
plt.axvline(0, color='black', linewidth=0.8, linestyle="--")
plt.axis('equal')
plt.legend()
plt.grid(alpha=0.3)
plt.savefig('Theory and sim.jpeg')


