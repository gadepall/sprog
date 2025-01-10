import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
parabola_lib = ctypes.CDLL('./parabola_functions.so')

# Define argument and return types for the C functions
parabola_lib.generate_parabola_1.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, 
                                              ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int)]
parabola_lib.generate_parabola_2.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, 
                                              ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int)]
parabola_lib.calculate_area.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), 
                                         ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
parabola_lib.calculate_area.restype = ctypes.c_double

# Define the range and step for generating points
start, end, step = 0.0, 1.0, 0.01  # Update range to ensure x >= 0 for the second parabola
n_points = int((end - start) / step) + 1

# Prepare arrays to hold the points
x_points1 = np.zeros(n_points, dtype=np.double)
y_points1 = np.zeros(n_points, dtype=np.double)
x_points2 = np.zeros(n_points, dtype=np.double)
y_points2 = np.zeros(n_points, dtype=np.double)

# Get points for both parabolas
n1 = ctypes.c_int(0)
n2 = ctypes.c_int(0)
parabola_lib.generate_parabola_1(start, end, step, x_points1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), 
                                  y_points1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), ctypes.byref(n1))
parabola_lib.generate_parabola_2(start, end, step, x_points2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), 
                                  y_points2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), ctypes.byref(n2))

# Calculate the area between the parabolas
area = parabola_lib.calculate_area(x_points1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), 
                                   y_points1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), 
                                   x_points2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), 
                                   y_points2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)), n1.value)

# Print the area
print(f"Area between the parabolas: {area:.6f}")

# Plot the parabolas and shade the area between them
x_vals = np.linspace(start, end, n_points)

plt.figure(figsize=(8, 6))
plt.plot(x_vals, x_vals**2, label='$y = x^2$', color='blue')
plt.plot(x_vals, np.sqrt(x_vals), label='$y^2 = x$', color='green')

# Fill the area between the curves
plt.fill_between(x_vals, x_vals**2, np.sqrt(x_vals), color='yellow', alpha=0.3)

# Mark the points (0, 0) and (1, 1)
plt.scatter([0, 1], [0, 1], color='red', zorder=5)
plt.text(0, 0, '(0, 0)', fontsize=12, ha='left', color='black')
plt.text(1, 1, '(1, 1)', fontsize=12, ha='left', color='black')

# Title and labels
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.show()

