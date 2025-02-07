import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./code.so')

# Define argument and return types for generate_pointsf_1
lib.generate_pointsf_1.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Pointer to the points array
    ctypes.c_int,                     # Number of points
    ctypes.POINTER(ctypes.c_double)   # Pointer to store the area
]
lib.generate_pointsf_1.restype = None

# Define argument and return types for generate_pointsf_2
lib.generate_pointsf_2.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Pointer to the points array
    ctypes.c_int,                     # Number of points
    ctypes.POINTER(ctypes.c_double)   # Pointer to store the area
]
lib.generate_pointsf_2.restype = None

# Number of points
num_points = 4000

# Create NumPy arrays for points
points_f1 = np.zeros((num_points, 2), dtype=np.double)
points_f2 = np.zeros((num_points, 2), dtype=np.double)

# Variable to store the area (integral) results
area_f1 = ctypes.c_double(0)
area_f2 = ctypes.c_double(0)

# Call generate_pointsf_1
lib.generate_pointsf_1(
    points_f1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points,
    ctypes.byref(area_f1)
)

# Call generate_pointsf_2
lib.generate_pointsf_2(
    points_f2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points,
    ctypes.byref(area_f2)
)

# Output the areas (integrals)
print(f"Area under f1(x) = |x^2 + 1| over [0, 1]: {area_f1.value}")
print(f"Area under f2(x) = |x + 1| over [1, 2]: {area_f2.value}")

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

# Add the area annotations inside the plot
# f1(x) = |x^2 + 1|
plt.text(0.5, 2.5, f'Area under f1: {area_f1.value:.2f}', color='red', fontsize=12, ha='center')

# f2(x) = |x + 1|
plt.text(1.5, 3, f'Area under f2: {area_f2.value:.2f}', color='blue', fontsize=12, ha='center')

# Customize plot
plt.grid(True)
ax.set_xlim(0, 2)
ax.set_ylim(0, 4)
plt.legend(loc="best")
ax.set_xlabel('X')
ax.set_ylabel('Y')
plt.title('Plots of x^2 + 1 and x + 1 with shaded areas')
plt.savefig('functions_plot_with_area.png')
plt.show()

