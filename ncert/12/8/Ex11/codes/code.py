import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math

# Load the shared library
lib = ctypes.CDLL('./code.so')  # Replace with the actual path of code.so

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

# Function to compute y = sqrt(4 * a * x) for the positive branch (upper part)
def f_positive(x):
    return np.sqrt(4 * a * x)

# Function to compute y = -sqrt(4 * a * x) for the negative branch (lower part)
def f_negative(x):
    return -np.sqrt(4 * a * x)

# Calculate the area using the C function
a = 1  # You can change this value for different values of 'a'
h = 0.01  # Step size for the trapezoidal method
area = abs(compute_area_python(h))  # Area calculated by the C function
print(f"Calculated Area: {area}")

# Plotting
x = np.arange(0, a, h)  # x values from 0 to a
y_positive = f_positive(x)  # y values from the positive branch
y_negative = f_negative(x)  # y values from the negative branch

# Plot the function and the area under both curves (positive and negative)
plt.figure(figsize=(8, 5))
plt.plot(x, y_positive, label='y^2=4ax', color='blue')
plt.plot(x, y_negative, color='blue')

plt.fill_between(x, y_positive, color='lightblue', alpha=0.5)
plt.fill_between(x, y_negative, color='lightblue', alpha=0.5, label='Area Under Curve')

plt.xlabel('x')
plt.ylabel('f(x) = ±sqrt(4 * a * x)')
plt.legend()
plt.grid(True)
plt.show()

