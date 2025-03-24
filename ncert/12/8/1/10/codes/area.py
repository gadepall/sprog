import numpy as np
import matplotlib.pyplot as plt
import os
import ctypes

dll = ctypes.CDLL('./area.so')

dll.generate_points.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.generate_points.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.calculate_area.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.calculate_area.restype = ctypes.c_float

dll.generate_parabola_points.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.generate_parabola_points.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.free_allocated_memory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.free_allocated_memory.restype = None

h = 0.0001  # Step size
x = -1     # Initial value of x
n = 2000    # Number of points to plot
a = 0.25    # 'a' in 4ay = x^2
n_area = 10000  # Number of points for area computation

'''COMPUTATIONAL SOLUTION'''
area = dll.calculate_area(x, h, n_area)
print("Area enclosed between the two curves: ", area)

'''PLOT'''
h = 0.01

'''PARABOLA'''
# Generating points on the parabola using the C function
parabola_points = dll.generate_parabola_points(x, h, a, n)

# Receiving the coordinates and storing them in a numpy array
parabola_coordinates = []
for point in parabola_points[:2 * n]:
    parabola_coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

parabola_coordinates = np.block(parabola_coordinates)

'''LINE'''
# Generating evenly spaced points using the C function
line_points = dll.generate_points(h, x, n)

# Receiving the coordinates and storing them in a numpy array
line_coordinates = []
for point in line_points[:2*n]:
    line_coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

line_coordinates = np.block(line_coordinates)

# Extract x and y coordinates for the parabola and line
x_parabola = parabola_coordinates[0, :]
y_parabola = parabola_coordinates[1, :]
x_line = line_coordinates[0, :]
y_line = (x_line+2)/4

# Plotting Points
plt.scatter(x_line, y_line, label="Line", color="blue")
plt.fill_between(x_parabola, y_parabola, y_line, where=(y_line > y_parabola), color="yellow", alpha=0.5)
plt.scatter(x_parabola, y_parabola, label="Parabola", color="red")

# Freeing up allocated memory
dll.free_allocated_memory(line_points, n)
dll.free_allocated_memory(parabola_points, n)

# Plot-related settings
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-5, 5)
plt.ylim(0, 8)
plt.legend()
plt.grid(True)
plt.show()

