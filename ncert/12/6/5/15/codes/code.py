import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the compiled C shared library
# Make sure to compile the C code into a shared library, e.g., `gcc -shared -o gradient.so -fPIC gradient.c`
c_library = ctypes.CDLL('./code.so')

# Define the C functions' argument and return types
c_library.func.restype = ctypes.c_double
c_library.func.argtypes = [ctypes.c_double]

c_library.funcder.restype = ctypes.c_double
c_library.funcder.argtypes = [ctypes.c_double]

c_library.gradientaccent.restype = ctypes.c_double
c_library.gradientaccent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]

# Parameters for gradient descent
initial_y = 0  # Starting point
h = 1e-10      # Learning rate
max_y = 35.0     # Upper limit for y
tolerance = 1e-6 # Convergence tolerance

# Call the gradient descent function in C to find the maximum point
optimal_y = c_library.gradientaccent(initial_y, h, max_y, tolerance)

# Call the C function to compute the function value at the maximum point
optimal_value = c_library.func(optimal_y)

# Print the maximum point
print(f"Maximum Point: y = {optimal_y:.6f}, f(y) = {optimal_value:.6f}")

# Generate data for plotting the function
y_values = np.linspace(0, max_y, 1000)
f_values = [c_library.func(y) for y in y_values]

# Plot the function
plt.figure(figsize=(12, 6))
plt.plot(y_values, f_values, label="f(y) = y^7 - 70y^6 + 1225y^5", color="blue")
plt.scatter(optimal_y, optimal_value, color="red", label=f"Maximum Point: (y = {optimal_y:.3f}, f(y) = {optimal_value:.3f})", zorder=5)
plt.title("Graph of f(y) from y=0 to y=35 with Maximum Point")
plt.xlabel("y")
plt.ylabel("f(y)")
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(optimal_y, color="red", linewidth=0.5, linestyle="--", alpha=0.7)
plt.legend()
plt.grid(True)
plt.show()
