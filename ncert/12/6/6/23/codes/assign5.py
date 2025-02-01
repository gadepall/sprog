import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared object file
lib = ctypes.CDLL('./gradient_descent.so')

# Define the argument types for the shared C function
lib.get_normal.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]

# Create an array for the results
x0_result = ctypes.c_double()
y0_result = ctypes.c_double()

# Call the C function to get the normal line results
lib.get_normal(ctypes.byref(x0_result), ctypes.byref(y0_result))

# Theoretical curve: y = x^2 / 4
x_vals = np.linspace(-3, 3, 100)
y_vals_curve = x_vals**2 / 4

# Theoretical normal: x + y = 3
y_vals_theoretical_normal = 3 - x_vals

# Simulated normal from gradient descent
normal_slope = -2 / x0_result.value
normal_intercept = (2 * x0_result.value / x0_result.value) + y0_result.value
y_vals_simulated_normal = normal_slope * x_vals + normal_intercept

# Plot the curve, theoretical normal, and simulated normal
plt.figure(figsize=(8, 6))

# Plot the curve
plt.plot(x_vals, y_vals_curve, label="Curve: $x^2 = 4y$", color="green")

# Plot the theoretical normal
plt.plot(x_vals, y_vals_theoretical_normal, label="Theoretical Normal: $x + y = 3$", color="red", linestyle="--")

# Plot the simulated normal (from gradient descent)
plt.plot(x_vals, y_vals_simulated_normal, label="Simulated Normal (Gradient Descent)", color="blue", linestyle=":")

# Labels
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.savefig('./figs/curve.png')


