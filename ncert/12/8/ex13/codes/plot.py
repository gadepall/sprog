import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared library
lib = ctypes.CDLL('./lib.so')

# Set argument and return types for the C functions
lib.trapezoidal_1.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_double]
lib.trapezoidal_2.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_double]
lib.function_1.argtypes=[ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
lib.function_2.argtypes=[ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]

# Parameters
x_start = 0
x_end = 4
h = 0.1
n_steps = 41

#Setting up the arrays
area_x = np.linspace(x_start, x_end, n_steps)
area_y_1 = np.zeros(n_steps)
area_y_2 = np.zeros(n_steps)
y_1=np.zeros(n_steps)
y_2=np.zeros(n_steps)

#Conversion to ctypes array
area_x_ctypes = area_x.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
area_y_1_ctypes = area_y_1.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
area_y_2_ctypes = area_y_2.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_1_ctypes = y_1.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_2_ctypes = y_2.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the C functions
lib.function_1(area_x_ctypes, y_1_ctypes, n_steps)
lib.trapezoidal_1(area_x_ctypes,area_y_1_ctypes, n_steps,h)
lib.function_2(area_x_ctypes, y_2_ctypes, n_steps)
lib.trapezoidal_2(area_x_ctypes,area_y_2_ctypes, n_steps,h)

Area1=area_y_1[40]
Area2=area_y_2[40]

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(area_x, y_1, label="Function1", linestyle='-', color='b',linewidth=7)
plt.plot(area_x, y_2, label="Function2", linestyle='-', color='r',linewidth=7)
plt.fill_betweenx(y_2, area_x, np.sqrt(4 * y_2), where=(y_2 >= 0), color='skyblue', alpha=0.5)
plt.fill_between(area_x, y_1, 0, where=(area_x >= 0), color='lightgreen', alpha=0.5)
plt.fill_betweenx(y_2, 0, area_x, where=(y_2 >= 0), color='lightcoral', alpha=0.5)


plt.xlabel("x")
plt.ylabel("y")
#plt.legend()
plt.legend(['$x^2=4y$','$y^2=4x$',f'RegionA(area={Area2-Area1})',f'RegionB(area={Area1})',f'RegionC(area={16-Area2})'])
plt.grid()
#plt.show()
plt.savefig('plot.png')

