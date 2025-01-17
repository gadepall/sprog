import numpy as np
import matplotlib.pyplot as plt
import math

# Function to compute the volume of a cylinder given its height h and radius R of the sphere
def compute_volume(h, R):
    r = np.sqrt(R**2 - (h**2) / 4)
    return np.pi * r**2 * h

# Function to compute the derivative of the volume with respect to h
def dV_dh(h, R):
    return np.pi * (R**2 - (3 * h**2) / 4)

# Gradient ascent method using matrices
def gradient_ascent(h0, alpha, R, tol, max_iter):
    h = np.array([h0])  # Matrix for storing height (1x1)
    gradient = np.array([0.0])  # Matrix for storing gradient (1x1)
    
    for _ in range(max_iter):
        gradient[0] = dV_dh(h[0], R)  # Gradient calculation at h[0]
        h[0] = h[0] + alpha * gradient[0]  # Update height based on gradient ascent
        if abs(gradient[0]) < tol:
            return h[0]  # Return the optimal height if the change is small enough
    
    return h[0]  # Return the final height after max iterations

# Parameters
R = 1.0        # Radius of the sphere
alpha = 0.001  # Learning rate
h0 = 1.0       # Initial guess for height
max_iter = 100000  # Maximum number of iterations
tol = 1e-6      # Tolerance level

# Run gradient ascent
optimal_height = gradient_ascent(h0, alpha, R, tol, max_iter)

# Compute the maximum volume corresponding to the optimal height
optimal_volume = compute_volume(optimal_height, R)

print(f" height (h) = {optimal_height:.6f}")
print(f"Maximum volume = {optimal_volume:.6f}")

# Plotting the result
heights = np.linspace(0, 2 * R, 500)
volumes = np.array([compute_volume(h, R) for h in heights])

plt.plot(heights, volumes, label="V = $\pi$r^2h")
plt.scatter(optimal_height, optimal_volume, color="red", label="Maximum point")
plt.xlabel("X")
plt.ylabel("Y")
plt.legend()
plt.grid(True)
plt.show()

