import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
solution_lib = ctypes.CDLL('./solution.so')

# Define the function signature from the shared library
# Assuming the function returns void and modifies an array passed by reference
solution_lib.solve_system.argtypes = [ctypes.POINTER(ctypes.c_double)]
solution_lib.solve_system.restype = None

# Create an array to hold the result
result = (ctypes.c_double * 2)()

# Call the function from the shared library to get the solution
solution_lib.solve_system(result)

# Extract the solution
intersection_point = [result[0], result[1]]
print(f"Intersection point from C code: {intersection_point}")

# Define the lines
x = np.linspace(-5, 5, 400)  # X range for plotting

# Line equations
y1 = (5 - 3 * x) / 2  # From 3x + 2y = 5
y2 = (2 * x - 7) / 3  # From 2x - 3y = 7

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x, y1, label='3x + 2y = 5', color='blue')
plt.plot(x, y2, label='2x - 3y = 7', color='green')

# Plot the intersection point
plt.scatter(intersection_point[0], intersection_point[1], color='red', label='Intersection Point')

# Add labels, legend, and grid
plt.xlabel('x')
plt.ylabel('y')
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
plt.legend()

# Show the plot
plt.show()
