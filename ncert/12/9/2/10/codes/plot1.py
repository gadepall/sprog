import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./libgenpoints.so') #Loading the shared .so file

# Defining the function  to get points 
lib.get_points.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int,
                           ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib.get_points.restype = ctypes.c_int  # Returning the type of the points from the function 

# Setting initial values 
initial_x = -1 #x_0
final_x = 1 
step_size = 0.01 #h

# Allocatting the points to arrays
num_points = int((final_x - initial_x) / step_size + 1)
x_vals = (ctypes.c_double * num_points)()
y_vals = (ctypes.c_double * num_points)()

# Generating the  positive semicircle
num_points_pos = lib.get_points(initial_x, final_x, step_size, 1, x_vals, y_vals)

# Converting the C arrays to Python lists
x_list_pos = [x_vals[i] for i in range(num_points_pos)]
y_list_pos = [y_vals[i] for i in range(num_points_pos)]

# Generating the negative semicircle
num_points_neg = lib.get_points(initial_x, final_x, step_size, 0, x_vals, y_vals)

# Converting again the C arrays to Python lists
x_list_neg = [x_vals[i] for i in range(num_points_neg)]
y_list_neg = [y_vals[i] for i in range(num_points_neg)]

# Parameters for the circle(Theoritical solution)
theta = np.linspace(0, 2 * np.pi, 1000)  #generating points for circle
x = np.cos(theta)  # x-coordinates of the unit circle
y = np.sin(theta)  # y-coordinates of the unit circle



# Plot the points and circle
plt.figure(figsize=(6, 6))
plt.plot(x, y, 'r', label="Theory")
plt.plot(x_list_pos, y_list_pos, 'bo--', markersize=0.7, label='Sim')
plt.plot(x_list_neg, y_list_neg, 'bo--', markersize=0.7, )
plt.gca().set_aspect('equal', adjustable='box')
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
plt.grid(True)
plt.savefig('../plots/plot.png', format='png', bbox_inches='tight')

