import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
sine_lib = ctypes.CDLL('./sine_area.so')

# Define the function signature
sine_lib.compute_sine.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
sine_lib.compute_sine.restype = ctypes.c_double

# Number of points
n = 1000
x = (ctypes.c_double * n)()
y = (ctypes.c_double * n)()

# Call the C function
area = sine_lib.compute_sine(x, y, n)
print(f"Computed Area: {area:.6f}")

# Convert the C arrays to NumPy arrays for plotting
x_vals = np.array(x[:n])
y_vals = np.array(y[:n])

# Plot the sine curve and the area
plt.figure(figsize=(8, 6))
plt.plot(x_vals, y_vals, label="y = sin(x)", color='blue')
plt.fill_between(x_vals, y_vals, alpha=0.3, label=f"Area ≈ {area:.6f}", color='cyan')
plt.title("Sine Curve and Area Under the Curve")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.show()

