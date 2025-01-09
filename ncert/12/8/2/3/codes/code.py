import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./code.so')  # Replace with the actual path

# Define the argument and return types of the compute_area function
lib.compute_area.argtypes = [ctypes.c_float, ctypes.POINTER(ctypes.c_float)]
lib.compute_area.restype = None

def compute_area_python(h):
    # Define a float to store the area
    area = ctypes.c_float(0.0)

    # Call the C function
    lib.compute_area(ctypes.c_float(h), ctypes.byref(area))

    # Return the computed area
    return area.value

# Define the function f(x) in Python for plotting
def f(x):
    return -x * x + x - 2

# Calculate the area using the C function
h = 0.1  # Step size
area = abs(compute_area_python(h))
print(f"Calculated Area: {area}")

# Plotting
x = np.arange(0, 3, h)
y = f(x)

plt.figure(figsize=(8, 5))
plt.plot(x, y, label='f(x)', color='blue')
plt.fill_between(x, y, color='lightblue', alpha=0.5, label='Area Under Curve')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid(True)
plt.show()

