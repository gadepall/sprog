import numpy as np
import matplotlib.pyplot as plt

# Define the volume function and its derivative
def V(x):
    return 1080 * x - 138 * x**2 + 4 * x**3

def V_prime(x):
    return 1080 - 276 * x + 12 * x**2

# Gradient ascent parameters
alpha = 0.01
epsilon = 1e-6
x_n = 1  # initial guess

# Gradient ascent loop
while True:
    x_next = x_n + alpha * V_prime(x_n)
    if abs(x_next - x_n) < epsilon:
        break
    x_n = x_next

# Found maximum
max_x = x_n
max_volume = V(max_x)

# Generate data for plotting
x_values = np.linspace(0, 12, 500)
V_values = V(x_values)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(x_values, V_values, label="Volume Curve", color="blue")
plt.scatter(max_x, max_volume, color="red", label=f"Maxima at x={max_x:.2f}, V={max_volume:.2f}")
plt.axvline(x=12, color="gray", linestyle="--", label="Constraint: 0<x<12")
plt.axvline(x=0, color="gray", linestyle="--")
plt.xlabel("x")
plt.ylabel("V(x)")
plt.legend()
plt.grid()
plt.show()

