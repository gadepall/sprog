import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared C library
grad_descent_lib = ctypes.CDLL('./gradient_descent.so')

# Define the argument and return types for the gradient descent function
grad_descent_lib.gradient_descent.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                                              ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
grad_descent_lib.gradient_descent.restype = None

# Constants
l = 10  # cm (fixed value of l)

# Function to calculate volume given r
def calculate_volume(r):
    return (1/3) * np.pi * r**2 * np.sqrt(l**2 - r**2)

# Generate values for r (from 0 to l)
r_values = np.linspace(0.1, l - 0.1, 500)  # Avoid r=0 to prevent division by zero

# Calculate volumes for each r value
volumes = np.array([calculate_volume(r) for r in r_values])

# Calculate r/(l^2 - r^2)^(1/2) ratio for each r
r_ratio_values = r_values / np.sqrt(l**2 - r_values**2)

# Plot the graph of Volume vs r/(l^2 - r^2)^(1/2)
plt.plot(r_ratio_values, volumes, label=r"Volume vs $\frac{r}{\sqrt{l^2 - r^2}}$")

# Find the maximum and minimum volume points
max_volume_index = np.argmax(volumes)
min_volume_index = np.argmin(volumes)
max_r_ratio = r_ratio_values[max_volume_index]
min_r_ratio = r_ratio_values[min_volume_index]
max_volume = volumes[max_volume_index]
min_volume = volumes[min_volume_index]

# Mark the max and min points on the graph
plt.scatter(max_r_ratio, max_volume, color='red', label=f"Max Volume ($r/\sqrt{{l^2 - r^2}} = {max_r_ratio:.2f}$)")
plt.scatter(min_r_ratio, min_volume, color='blue', label=f"Min Volume ($r/\sqrt{{l^2 - r^2}} = {min_r_ratio:.2f}$)")

# Gradient Descent - Calling C function
optimal_r_max = ctypes.c_double()
optimal_volume_max = ctypes.c_double()
optimal_r_min = ctypes.c_double()
optimal_volume_min = ctypes.c_double()

grad_descent_lib.gradient_descent(ctypes.byref(optimal_r_max), ctypes.byref(optimal_volume_max),
                                  ctypes.byref(optimal_r_min), ctypes.byref(optimal_volume_min))

# Plot the optimal points found using gradient descent
plt.scatter(optimal_r_max.value / np.sqrt(l**2 - optimal_r_max.value**2), optimal_volume_max.value, color='green', label=f"GD Max Volume ($r/\sqrt{{l^2 - r^2}} = {optimal_r_max.value / np.sqrt(l**2 - optimal_r_max.value**2):.2f}$)")
plt.scatter(optimal_r_min.value / np.sqrt(l**2 - optimal_r_min.value**2), optimal_volume_min.value, color='purple', label=f"GD Min Volume ($r/\sqrt{{l^2 - r^2}} = {optimal_r_min.value / np.sqrt(l**2 - optimal_r_min.value**2):.2f}$)")

# Labels and title
plt.xlabel(r'$\frac{r}{\sqrt{l^2 - r^2}}$ ratio')
plt.ylabel(r'Volume $(\mathrm{cm}^3)$')
plt.title(r'Volume vs $\frac{r}{\sqrt{l^2 - r^2}}$ ratio ($l = 10$ cm)')

# Show the plot with grid and legend
plt.grid(True)
plt.legend()
plt.show()

# Print results for the gradient descent method
print(f"Max volume found at r = {optimal_r_max.value:.2f} cm with volume = {optimal_volume_max.value:.2f} cm^3")
print(f"Min volume found at r = {optimal_r_min.value:.2f} cm with volume = {optimal_volume_min.value:.2f} cm^3")

