import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# Load the shared library
gen = ctypes.CDLL('./curve.so')

# Define C function signatures
gen.generate_conic.argtypes = [
    ctypes.c_int,  # size
    ctypes.c_double,  # a
    ctypes.c_double,  # b
    ctypes.POINTER(ctypes.c_double),  # c_x_coord
    ctypes.POINTER(ctypes.c_double),  # y_coord_pos
    ctypes.POINTER(ctypes.c_double),  # y_coord_neg
]
gen.generate_conic.restype = None

gen.generate_line.argtypes = [
    ctypes.c_int,  # size
    ctypes.c_double,  # a
    ctypes.c_double,  # b
    ctypes.POINTER(ctypes.c_double),  # x_coord
    ctypes.POINTER(ctypes.c_double),  # y_coord
]
gen.generate_line.restype = None

gen.area.argtypes = [
    ctypes.c_int,  # size
    ctypes.c_double,  # a
    ctypes.c_double,  # b
    ctypes.c_double,
    ctypes.c_double,
]
gen.area.restype = ctypes.c_double

#parameters
size = 4000
a = 2.0
b = 6.0
x_0 = 0.0 
x_n = 2.0
trapezoids = 20000

# Allocate memory for arrays (size + 1 to match C loop bounds)
c_x_coord = np.zeros((size + 1 , 1), dtype = np.double)
y_coord_pos = np.zeros((size + 1 , 1), dtype = np.double)
y_coord_neg = np.zeros((size + 1 , 1), dtype = np.double)
x_coord = np.zeros((size + 1 , 1), dtype = np.double)
y_coord = np.zeros((size + 1 , 1), dtype = np.double)

# Call the C functions
gen.generate_conic(size, a, b, c_x_coord.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), y_coord_pos.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), y_coord_neg.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
gen.generate_line(size, a, b, x_coord.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), y_coord.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
A = gen.area(trapezoids, a, b, x_0, x_n)


#print the areas 
print(f"Theoretical Area: {3*math.pi - 6}")
print(f"Computed Area: {A}")

# Plot the ellipse
plt.plot(c_x_coord, y_coord_pos, label=r'ellipse: $\frac{x^2}{4} + \frac{y^2}{36} = 1$', color='blue')
plt.plot(c_x_coord, y_coord_neg, color='blue')

# Plot the line
plt.plot(x_coord, y_coord, label=r'line: $\frac{x}{2} + \frac{y}{6} = 1$', color='green')

# Define plot range for the fill
plot_x_vals = c_x_coord.flatten()  # Flatten the array for plotting
plot_y_pos_vals = y_coord_pos.flatten()
plot_y_neg_vals = y_coord_neg.flatten()
line_y_vals = np.interp(plot_x_vals, x_coord.flatten(), y_coord.flatten())  # Interpolate line values

# Ensure we focus only on the intersection region
fill_mask = (plot_x_vals >= x_0) & (plot_x_vals <= x_n)  # Restrict x values
ellipse_mask = (plot_y_pos_vals >= line_y_vals)  # Where ellipse is above the line

# Combined mask for the filled region
combined_mask = fill_mask & ellipse_mask

plt.fill_between(
    plot_x_vals,
    line_y_vals,
    plot_y_pos_vals,
    where=combined_mask,
    color='pink',
    alpha=0.5,
    label='Shaded Area'
)

n = int(size/2)
plt.plot(x_0, y_coord_pos[n], 'bo', label = '(0, 2)')
plt.plot(x_n, y_coord_pos[size], 'ro', label = '(6, 0)')

ax = plt.gca()
plt.grid()
ax.set_xlim(-(b + 1) , b + 1)
ax.set_ylim(-(b + 1) , b + 1)
plt.legend(loc="best")
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.savefig('../figs/fig.png')
plt.show()
