import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
c_lib = ctypes.CDLL('./code.so')  # Replace with the actual name of your .so file

# Define argument and return types for the 'min' function
c_lib.min.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]

# Define the function y = 9x^2 + 12x + 2
def func_y(x):
    return 9 * x**2 + 12 * x + 2

# Initialize x and y
x = ctypes.c_float(-10)  # Starting value for x
y = ctypes.c_float(0)    # Placeholder for y

# Call the C function to find the minimum
c_lib.min(ctypes.byref(x), ctypes.byref(y))

# Extract results from C function
min_x = x.value
min_y = y.value
print({min_x})

print(f"Minimum value found at x = {min_x}, y = {min_y}")

# Generate x values for the plot
x_vals = np.linspace(-15, 5, 500)
y_vals = func_y(x_vals)

# Plot the graph
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label='graph of given function', color='blue')
plt.scatter(min_x, min_y, color='red', label='minimum(-0.6666,-2)', zorder=5)
plt.xlabel('x')
plt.ylabel('y')
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')
plt.legend()
plt.grid(True)
plt.show()

