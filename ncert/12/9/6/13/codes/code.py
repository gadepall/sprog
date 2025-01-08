import ctypes
import matplotlib.pyplot as plt
import numpy as np

# Load the shared library
lib = ctypes.CDLL('./lib.so')

# Define the function prototype
lib.y_values.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]
lib.y_values.restype = ctypes.c_double

# Parameters
x_0 = np.pi/3.0 # initial x value
y_0 = 0 #initial y value
h = 0.01

# List to store values
x_itr = [x_0]
y_itr = [y_0]

# Perform Euler's method using the C function
for i in range(100):
    x_i = x_itr[-1]
    y_i = y_itr[-1]
    x_next = x_i + h
    y_next = lib.y_values(y_i, h, x_i) 
    x_itr.append(x_next)
    y_itr.append(y_next)

# Theoretical solution
def theo_result(x):
    return (np.cos(x) - 2*((np.cos(x))**2))

# Calculating values
x_theory = np.linspace(x_itr[0], x_itr[-1], 1000)  # Range of x for plotting
y_theory = theo_result(x_theory)

# Plot the results
plt.plot(x_itr, y_itr, label = "Sim" , color="red")
plt.plot(x_theory, y_theory, label = "Theory" , color="green")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()
plt.grid(True)
plt.savefig('../figs/fig.png')
plt.show()

