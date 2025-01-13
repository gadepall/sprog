import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared library
trapezoidal = ctypes.CDLL('./trapezoidal.so')

# Set argument and return types for the C functions
trapezoidal.trapezoidal.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_double]
trapezoidal.function.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]

# Parameters
x_start = 0
x_end = 5
h = 0.1
n_steps = 51

#Intialising the x and y arrays
x = np.linspace(x_start, x_end, n_steps)
y = np.zeros(n_steps)
y_trapezoidal=np.zeros(n_steps)

#Converting array to ctypes
x_ctypes = x.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_ctypes = y.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_trapezoidal_ctypes = y_trapezoidal.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the C functions
trapezoidal.function(x_ctypes, y_ctypes, n_steps)
trapezoidal.trapezoidal(x_ctypes, y_ctypes,y_trapezoidal_ctypes, n_steps,h)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(x, y, label="Theory", linestyle='-', color='b',linewidth=10)
plt.plot(x, y_trapezoidal, label="Trapezoidal", linestyle='--', color='r',linewidth=7)
plt.xlabel("x")
plt.ylabel("y")
#plt.legend()
plt.legend(['Theory','Trapezoidal'])
plt.grid()
#plt.show()
plt.savefig('plot.png')

