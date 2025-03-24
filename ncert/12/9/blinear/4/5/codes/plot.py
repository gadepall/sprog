import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Loading the shared library
lib = ctypes.CDLL('./libeuler.so')

# Defining the function signature for euler function
lib.Trapezoid.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                             ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib.Trapezoid.restype = None

# Set initial values
x0 = -5       # Initial x
y0 = 5.0005      # Initial y
x_final = 5.0      # End value of x
step_size = 0.01  # Step size

# Calculating the number of points
num_points = int((x_final - x0) / step_size) + 1

# Creating arrays to store x and y values
x_vals = (ctypes.c_double * num_points)()
y_vals = (ctypes.c_double * num_points)()

# Calling the C function to fill the arrays
lib.Trapezoid(x0, y0, x_final, step_size, x_vals, y_vals)

# Converting the C arrays to Python lists for plotting
x_list = [x_vals[i] for i in range(num_points)]
y_list = [y_vals[i] for i in range(num_points)]

#Theory plotting
x = np.linspace(-5, 5, 5000)
y = np.log(np.exp(2 * x) + 1) - x

# Plot the points
plt.figure(figsize=(6, 6))
plt.plot(x, y, label=r"Theory", color="blue")
plt.plot(x_list, y_list, 'r--', markersize=0.75, label="Sim")
plt.xlabel('x')
plt.ylabel('y')
plt.grid(True)
plt.legend()
plt.savefig('../plots/plot.png', format='png', bbox_inches='tight')
plt.show()

