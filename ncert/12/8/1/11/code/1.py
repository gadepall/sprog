import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
trapezoidal = ctypes.CDLL('./c.so')

# Set argument and return types for the compute_total_area function
trapezoidal.compute_total_area.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
trapezoidal.compute_total_area.restype = ctypes.c_double

# Define parameters
a = 0.0  # Start of the interval
b = 3.0  # End of the interval
n = 1000  # Number of trapezoids

# Call the compute_total_area function from the C library
total_area = trapezoidal.compute_total_area(a, b, n)

# Multiplying the total_area so as to include both upper and lower halves
total_area *= 2  
# Print the total area
print(f"Total area (sim): {total_area}")

# Plotting the curve y^2 = 4x
x = np.linspace(a, b, 500)  # Generate x values
y = np.sqrt(4 * x)          # Calculate corresponding y values for the curve

# Create the plot
plt.figure(figsize=(8, 6))

# Plot the curve
plt.plot(x, y, label=r"$y^2 = 4x$", color="blue", linewidth=2)

# Plot the vertical line x = 3
plt.axvline(x=3, color="red", linestyle="--", label=r"$x = 3$")

# Fill the shaded region between the curve and x=3
plt.fill_betweenx(y, x, 3, where=(x <= 3), color="lightblue", alpha=0.5)


plt.xlabel("x", fontsize=12)
plt.ylabel("y", fontsize=12)
plt.legend(fontsize=12)

# Add the area annotation
plt.text(1.5, 3.5, f"sim Area = {total_area:.2f}", fontsize=12, color="black", bbox=dict(facecolor='white', alpha=0.5))

# Display the plot
plt.grid(True)
plt.show()

