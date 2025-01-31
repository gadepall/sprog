import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
newton_raphson_lib = ctypes.CDLL('./newton_raphson.so')

# Define function signatures
newton_raphson_lib.newton_raphson.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
newton_raphson_lib.newton_raphson.restype = ctypes.c_double

newton_raphson_lib.generate_quadratic_values.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # x_values
    ctypes.POINTER(ctypes.c_double),  # y_values
    ctypes.c_int                     # size
]

# Solve using Newton-Raphson
initial_guess = 10.0
tolerance = 1e-6
max_iter = 100

root = newton_raphson_lib.newton_raphson(initial_guess, tolerance, max_iter)
print(f"Root found using Newton-Raphson: {root:.6f}")

# Generate data for plotting
x_values = np.linspace(-30, 30, 500)
y_values = np.zeros_like(x_values)

# Convert arrays to ctypes
x_array = (ctypes.c_double * len(x_values))(*x_values)
y_array = (ctypes.c_double * len(y_values))()

# Call the function to generate y-values
newton_raphson_lib.generate_quadratic_values(x_array, y_array, len(x_values))

# Convert y_array back to numpy array
y_values = np.array(y_array)

# Plot the quadratic equation
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label="f(b) = b^2 - 400", color="blue")
plt.axhline(0, color="black", linewidth=0.8, linestyle="--")
plt.axvline(root, color="red", linestyle="--", label=f"Root: b = {root:.2f}")
plt.title("Quadratic Equation: f(b) = b^2 - 400")
plt.xlabel("b")
plt.ylabel("f(b)")
plt.legend()
plt.grid()
plt.show()
