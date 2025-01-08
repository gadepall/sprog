import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
circle_lib = ctypes.CDLL('./circle.so')

# Define the function signature for 'calculate_area' from the shared object
# The function is 'calculate_area(double *area, double radius, double x1, double x2, int n)'
circle_lib.calculate_area.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int]
circle_lib.calculate_area.restype = None

# Function to call the C function 'calculate_area' from the shared library
def get_area(radius, x1, x2, n):
    area = ctypes.c_double(0.0)
    circle_lib.calculate_area(ctypes.byref(area), radius, x1, x2, n)
    return area.value

# Define the circle function y = sqrt(radius^2 - x^2) for the plot
def circle_y(x, radius):
    return np.sqrt(radius**2 - x**2)

# Parameters
radius = 2.0
x1 = 0.0
x2 = 2.0
n = 1000

# Get the area using the C function from the shared object
area = get_area(radius, x1, x2, n)

print(f"Area in the first quadrant bounded by the circle: {area:.4f}")

# Plotting the circle and the area
# Create x values from 0 to 2 for the first quadrant
x_values = np.linspace(x1, x2, 1000)
y_values = circle_y(x_values, radius)

# Plot the circle in the first quadrant
plt.figure(figsize=(6, 6))
plt.plot(x_values, y_values, label="Circle: $x^2 + y^2 = 4$")
plt.fill_between(x_values, 0, y_values, color="lightblue", alpha=0.5, label="Area in the first quadrant")

# Plotting formatting
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.xlim(-0.1, 2.1)
plt.ylim(-0.1, 2.1)
plt.gca().set_aspect('equal', adjustable='box')
plt.title('Circle and Area in the First Quadrant')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()

