import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./libcurves.so')

# Define the Parabola and Circle structs
class Parabola(ctypes.Structure):
    _fields_ = [("x_values", ctypes.POINTER(ctypes.c_double)),
                ("y_values", ctypes.POINTER(ctypes.c_double)),
                ("num_points", ctypes.c_int),
                ("a", ctypes.c_double),
                ("x_min", ctypes.c_double),
                ("x_max", ctypes.c_double),
                ("V", ctypes.POINTER(ctypes.POINTER(ctypes.c_double))),
                ("u", ctypes.POINTER(ctypes.c_double)),
                ("f", ctypes.c_double)]

class Circle(ctypes.Structure):
    _fields_ = [("x_values", ctypes.POINTER(ctypes.c_double)),
                ("y_values", ctypes.POINTER(ctypes.c_double)),
                ("num_points", ctypes.c_int),
                ("r", ctypes.c_double),
                ("V", ctypes.POINTER(ctypes.POINTER(ctypes.c_double))),
                ("u", ctypes.POINTER(ctypes.c_double)),
                ("f", ctypes.c_double)]

# Define the C functions 
lib.create_parabola.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.c_double, ctypes.c_double]
lib.create_circle.argtypes = [ctypes.c_int, ctypes.c_double]
lib.gen_parab.argtypes = [ctypes.POINTER(Parabola)]
lib.gen_circle.argtypes = [ctypes.POINTER(Circle)]
lib.find_intersections.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(Parabola), ctypes.POINTER(Circle)]
lib.area_parab.argtypes = [ctypes.POINTER(Parabola), ctypes.c_double, ctypes.c_double]
lib.area_circle.argtypes = [ctypes.POINTER(Circle), ctypes.c_double, ctypes.c_double]

# Define the return types
lib.create_parabola.restype = ctypes.POINTER(Parabola)
lib.create_circle.restype = ctypes.POINTER(Circle)
lib.find_intersections.restype = None
lib.area_parab.restype = ctypes.c_double
lib.area_circle.restype = ctypes.c_double

# Parameters
num_points = 100000
a = 1.5  # For the parabola y^2 = 4ax
x_min = 0.0
x_max = 5.0
r = np.sqrt(16)  # Radius for the circle 4x^2 + 4y^2 = 9
intersection_points = (ctypes.c_double * 4)()  # Space for intersection points

# Create curves using the new structs
parabola = lib.create_parabola(num_points, a, x_min, x_max)
circle = lib.create_circle(num_points, r)

# Generate points for the parabola and circle
lib.gen_parab(parabola)
lib.gen_circle(circle)

# Find intersections
lib.find_intersections(intersection_points, parabola, circle)

# Extract intersection points
x1 = intersection_points[0]
y1_pos = intersection_points[1]
y1_neg = intersection_points[3]

# Calculating the total area
area_parab = lib.area_parab(parabola, x_min, x1)
area_circle = lib.area_circle(circle, x1, r)

total_area = area_parab + area_circle

print(f"Total area between the curves: {total_area}")

# Convert the pointers to numpy arrays
parabola_struct = parabola.contents  
circle_struct = circle.contents  

# Create numpy arrays from the struct fields
x_parab_np = np.ctypeslib.as_array(parabola_struct.x_values, shape=(num_points,))
y_parab_np = np.ctypeslib.as_array(parabola_struct.y_values, shape=(num_points,))

x_circle_np = np.ctypeslib.as_array(circle_struct.x_values, shape=(num_points,))
y_circle_np = np.ctypeslib.as_array(circle_struct.y_values, shape=(num_points,))

plt.figure(figsize=(6, 6))

# Plot parabola
plt.plot(x_parab_np, y_parab_np, label='Parabola: $y^2=6x$', color='blue')
plt.plot(x_parab_np, -y_parab_np, color='blue')  

# Plot circle
plt.plot(x_circle_np, y_circle_np, label='Circle: $x^2 + y^2 = 16$', color='orange')
plt.plot(x_circle_np, -y_circle_np, color='orange') 

# Plot intersection points
plt.plot(x1, y1_pos, 'ro') 
plt.text(x1, y1_pos, f'({x1:.2f}, {y1_pos:.2f})')

plt.plot(x1, y1_neg, 'ro')  
plt.text(x1, y1_neg, f'({x1:.2f}, {y1_neg:.2f})')

# Shading regions
shade_parabola = np.logical_and(x_parab_np >= x_min, x_parab_np <= x1)
shade_circle = np.logical_and(x_circle_np >= x1, x_circle_np <= r)

# Controlled shading
plt.fill_between(x_parab_np, y_parab_np, -y_parab_np, where=shade_parabola, color='pink', alpha=0.5)
plt.fill_between(x_circle_np, y_circle_np, 0, where=shade_circle, color='pink', alpha=0.5, label='Shaded Area (Circle & Parabola)')

plt.xlim(-6, 6)
plt.ylim(-6, 6)
plt.axhline(0, color='black', lw=0.5, ls='-')
plt.axvline(0, color='black', lw=0.5, ls='-')
plt.xlabel('x-axis')
plt.ylabel('y-axis')
plt.legend()
plt.grid()
plt.savefig('../figs/fig.png')
plt.show()

# Free allocated memory
lib.free_parabola(parabola)
lib.free_circle(circle)

