#Code by S.Sai Akshita
#importing libraries
import numpy as np
import matplotlib.pyplot as plt
import math
import ctypes


# Define the parameters
C = 1000  # Initial value (constant of proportionality)
t = np.linspace(0, 20, 500)  # Time range from 0 to 100 with 500 points
x=[0]
y=[1000]
h=1



#Calling C Function
function=ctypes.CDLL('./calc.so')
function.output.argtypes=[ctypes.c_double]
function.output.restype=ctypes.c_double


#Generating points on the graph using for loop
for i in range(0,20):
    x.append(float(x[i]+h))
    y.append(function.output(y[i]))
#end of for loop
x=np.array(x)
y=np.array(y)



# Exponential function for P
def P(t, C):
    return C * np.exp(t / 20)

# Calculate P(t)
P_values = P(t, C)

# Plot theoretical function
plt.figure(figsize=(10, 6))
plt.plot(t, P_values, label='Theory',linewidth='5', color="yellow")
plt.plot(x,y,label='Sim',color='Red')
plt.title("Exponential Growth of P over Time", fontsize=14)
plt.xlabel("Time (t)", fontsize=12)
plt.ylabel("P(t)", fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(fontsize=12)
plt.savefig('Principal.jpeg')

