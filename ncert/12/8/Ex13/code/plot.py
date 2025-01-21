import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library (adjust path to the correct location)
lib = ctypes.CDLL('./main.so')  # Replace with the correct path to your .so file

# Define argument and return types for the C functions
lib.trapezoidal_mod_cos.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.trapezoidal_mod_cos.restype = ctypes.c_double

# Set the initial values and parameters
x1 = 0.0  # Start of interval
x2 = 2 * np.pi  # End of interval
n = int(2 * np.pi / 0.00001)  # Number of intervals based on the step size

# Arrays to store results for plotting
x_vals = np.linspace(x1, x2, n)
cos_vals = np.cos(x_vals)
mod_cos_vals = np.abs(cos_vals)  # Take the absolute value of cos(x)

# Detect intersections of cos(x) with the x-axis (where cos(x) = 0)
intersections_cos_x = []
intersections_cos_y = []  # All intersections have y = 0


# Find intersections for cos(x)
for i in range(1, n):
    if cos_vals[i - 1] * cos_vals[i] < 0:  # Sign change indicates intersection
        intersections_cos_x.append(x_vals[i])
        intersections_cos_y.append(0)



# Plot the graph of cos(x)
plt.figure(figsize=(12, 8))
plt.plot(x_vals, cos_vals, label="cos(x)", color='b', linewidth=2)
plt.fill_between(x_vals, cos_vals, color='blue', alpha=0.3, label="Area under cos(x)")


# Plot and label the points of intersection for cos(x)
for x_int in intersections_cos_x:
    plt.scatter(x_int, 0, color='red', zorder=5)  # Plot the point
    plt.text(x_int, 0.05, f"({x_int:.2f}, 0)", color='blue', fontsize=10, ha='center')  # Label the point

# Plot settings
plt.xlim([0, 2 * np.pi])
plt.ylim([-1.1, 1.1])  # For cos(x)
plt.xlabel("x-axis")
plt.ylabel("cos(x) / |cos(x)|")
plt.axhline(0, color='black', linewidth=1)  # x-axis
plt.legend()
plt.grid(True)

# Show the plot
plt.show()

# Call the C function to calculate the area under |cos(x)|
area = lib.trapezoidal_mod_cos(x1, x2, n)
print(f"The area under |cos(x)| from 0 to 2π is: {area:.5f}")


