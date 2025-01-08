import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Loading the shared library
lib = ctypes.CDLL('./libeuler.so')

# Defining the function signature for euler function
lib.euler.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                             ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib.euler.restype = None

# Set initial values
x0 = -3       # Initial x
y0 =   2.31523      # Initial y
x_final = 3.0      # End value of x
step_size = 0.01  # Step size

# Calculating the number of points
num_points = int((x_final - x0) / step_size) + 1

# Creating arrays to store x and y values
x_vals = (ctypes.c_double * num_points)()
y_vals = (ctypes.c_double * num_points)()

# Calling the C function to fill the arrays
lib.euler(x0, y0, x_final, step_size, x_vals, y_vals)

# Converting the C arrays to Python lists for plotting
x_list = [x_vals[i] for i in range(num_points)]
y_list = [y_vals[i] for i in range(num_points)]

#Theory plotting
y = np.linspace(0.1, 10, 5000)  
# Computing x for the given equation x^2 = 2y^2 * ln(y)
x_squared = 2 * y**2 * np.log(y)

# Removing invalid values (x^2 must be non-negative)
valid_indices = x_squared >= 0
y = y[valid_indices]
x_squared = x_squared[valid_indices]

# Compute x as both positive and negative roots
x_positive = np.sqrt(x_squared)
x_negative = -np.sqrt(x_squared)

# Filter x values to be within the range -3 to 3
valid_x_indices = (x_positive <= 3) & (x_negative >= -3)

x_positive = x_positive[valid_x_indices]
x_negative = x_negative[valid_x_indices]
y = y[valid_x_indices]





# Plot the points
plt.figure(figsize=(6, 6))
plt.plot(x_positive, y, color='blue')
plt.plot(x_negative, y, label=r'Theory', color='blue')
plt.plot(x_list, y_list, 'r--', markersize=0.75, label="Sim")
plt.xlabel('x')
plt.ylabel('y')
plt.grid(True)
plt.legend()
plt.savefig('../plots/plot.png', format='png', bbox_inches='tight')

