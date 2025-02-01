import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./code.so')

# Define argument and return types for generate_pointsf_1
lib.generate_pointsf_1.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Pointer to the points array
    ctypes.c_int                     # Number of points
]
lib.generate_pointsf_1.restype = None

# Define argument and return types for calc_integralf_1
lib.calc_integralf_1.argtypes = [
    ctypes.c_int,                     # Number of points
    ctypes.POINTER(ctypes.c_double)   # Pointer to store the result
]
lib.calc_integralf_1.restype = None

# Define argument and return types for generate_pointsf_2
lib.generate_pointsf_2.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Pointer to the points array
    ctypes.c_int                     # Number of points
]
lib.generate_pointsf_2.restype = None

# Number of points
num_points = 4000

# Create NumPy arrays for points
points_f1 = np.zeros((num_points, 2), dtype=np.double)
points_f2 = np.zeros((num_points, 2), dtype=np.double)

# Call generate_pointsf_1
lib.generate_pointsf_1(
    points_f1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Call generate_pointsf_2
lib.generate_pointsf_2(
    points_f2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Plotting both functions with shaded areas
fig = plt.figure()
ax = plt.gca()

# Plot x^2 + 1
plt.plot(points_f1[:, 0], points_f1[:, 1], c='r', label="x^2 + 1 (interval [0, 1])")

# Shade the area under x^2 + 1
plt.fill_between(points_f1[:, 0], points_f1[:, 1], color='red', alpha=0.3)

# Plot x + 1
plt.plot(points_f2[:, 0], points_f2[:, 1], c='b', label="x + 1 (interval [1, 2])")

# Shade the area under x + 1
plt.fill_between(points_f2[:, 0], points_f2[:, 1], color='blue', alpha=0.3)

# Customize plot
plt.grid(True)
ax.set_xlim(0, 2)
ax.set_ylim(0, 4)
plt.legend(loc="best")
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.title('Plots of x^2 + 1 and x + 1 with shaded areas')
plt.savefig('functions_plot_shaded.png')
plt.show()

