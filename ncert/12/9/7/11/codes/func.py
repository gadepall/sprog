#importing required libraries
import matplotlib.pyplot as plt
import math
import numpy as np
from scipy.integrate import solve_ivp
from ctypes import *

#loading the shared object file 
func = CDLL('./func.so')

#assigning the c functions in python
x_new = func.x_new
y_new = func.y_new

#setting the datatype of the value returned by the function
x_new.restype=c_double
y_new.restype=c_double

#initialising the values of x_0,y_0 and h
x_0=0
y_0=-1
h=0.001

#plotting the theoretical graph
def diff_eq(t, y):
    x, y_val = t, y[0]  # x is t, y_val is the dependent variable
    dx_dt = ((x - y_val) - 1) / (-(x - y_val + 1))  # derived from the equation
    return [dx_dt]
    
initial_conditions = [-1]
x_vals = np.linspace(0,0.5,500)
solution = solve_ivp(diff_eq, [0, 10], initial_conditions, t_eval=x_vals)
plt.plot(solution.t, solution.y[0], 'ro',linewidth=1,label='Theoretical Plot')
 
#iterating to generate more points on the curve using the method of finite differences and plotting them
for i in range(500):
    x_0=x_new(c_double(x_0),c_double(h))
    y_0=y_new(c_double(x_0),c_double(y_0),c_double(h))
    plt.plot(x_0,y_0,'ko',markersize=2)

#setting up the grid,labels and legend 
plt.plot([], [], 'ko', label='Simulated Plot') 
plt.xlabel('X-Axis')
plt.ylabel('Y-Axis')
plt.grid(True)
plt.legend()

#saving the generated figure
plt.savefig('../figs/fig.pdf')






