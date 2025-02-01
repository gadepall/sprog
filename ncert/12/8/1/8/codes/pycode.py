import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library
lib = ctypes.CDLL('./funcs.so')  # Replace with the actual path of code.so

# Define the argument and return types of the compute_area function
lib.compute_area.argtypes = [ctypes.c_float, ctypes.POINTER(ctypes.c_float)]
lib.compute_area.restype = None

def compute_area_python(h):
    # Define a float to store the area
    area = ctypes.c_float(0.0)

    # Call the C function
    lib.compute_area(ctypes.c_float(h), ctypes.byref(area))

    # Return the computed area
    return 2*area.value

# Function to compute y = sqrt( x) for the positive branch (upper part)
def f_positive(x):
    return np.sqrt(  x)

# Function to compute y = -sqrt( x) for the negative branch (lower part)
def f_negative(x):
    return -np.sqrt( x)

# Calculate the area using the C function
a = 1  # You can change this value for different values of 'a'
h = 0.01  # Step size for the trapezoidal method
area = abs(compute_area_python(h))  # Area calculated by the C function
print(f"Calculated Area: {area}")

# Plotting
x_full = np.arange(0, 4, h)  # x values from 0 to 4 (extends the curve up to x=4)
y_positive_full = f_positive(x_full)  # y values from the positive branch
y_negative_full = f_negative(x_full)  # y values from the negative branch

# Plot the function and the area under both curves (positive and negative)
plt.figure(figsize=(8, 5))
plt.plot(x_full, y_positive_full, label='y^2=x', color='blue')
plt.plot(x_full, y_negative_full, color='blue')

# Set the x limits to stop the shading at x = 4
x_shade = np.arange(0, 4, h)  # Modify x range to stop at x = 4
y_positive_shade = f_positive(x_shade)
y_negative_shade = f_negative(x_shade)

# Fill the area with light green between x = 0 and x = 8/3
x_shade_green = np.arange(0, 8/3, h)
y_positive_shade_green = f_positive(x_shade_green)
y_negative_shade_green = f_negative(x_shade_green)

plt.fill_between(x_shade_green, y_positive_shade_green, color='lightgreen', alpha=0.5)
plt.fill_between(x_shade_green, y_negative_shade_green, color='lightgreen', alpha=0.5, label='Area Under Curve (Light Green)')

# Fill the area with light blue between x = 8/3 and x = 4
x_shade_blue = np.arange(8/3, 4, h)
y_positive_shade_blue = f_positive(x_shade_blue)
y_negative_shade_blue = f_negative(x_shade_blue)

plt.fill_between(x_shade_blue, y_positive_shade_blue, color='lightblue', alpha=0.5)
plt.fill_between(x_shade_blue, y_negative_shade_blue, color='lightblue', alpha=0.5, label='Area Under Curve (Light Blue)')

# Add vertical lines at x = 4 and x = 8/3
plt.axvline(x=4, color='red', linestyle='--', label="x = 4")
plt.axvline(x=8/3, color='green', linestyle='--', label="x = 8/3")

plt.xlabel('x')
plt.ylabel('f(x) = ±sqrt( x)')

# Place the legend in the upper right corner
plt.legend(loc='upper left')  # Change location of the legend here
plt.grid(True)
plt.show()

