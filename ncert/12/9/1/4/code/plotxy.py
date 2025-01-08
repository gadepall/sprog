import numpy as np
import matplotlib.pyplot as plt

# Define the parameters
h = 0.001  # Time step size (x-step size here)
T = 10    # Total time (x values)
n_steps = int(T / h)  # Number of time steps

# Initial conditions
y0 = 1  # Initial y value
v0 = 0  # Initial v = dy/dx value

# Initialize arrays for y, v, and x
y = np.zeros(n_steps + 1)
v = np.zeros(n_steps + 1)
x = np.linspace(0, T, n_steps + 1)

# Set initial values
y[0] = y0
v[0] = v0

# Define the system of equations: 
# dy/dx = v, dv/dx = -sqrt(-cos(v))
def dv_dx(v):
    cos_v = np.cos(v)
    if cos_v > 0:
        # In this case, let's avoid returning NaN by returning a small value
        # You can experiment with this value for better behavior.
        return -np.sqrt(np.abs(-cos_v))
    return -np.sqrt(-cos_v)  # dv/dx = -sqrt(-cos(v))

# Runge-Kutta 4th Order Method
for n in range(n_steps):
    # Calculate k1, k2, k3, and k4 for y and v
    k1_y = h * v[n]
    k1_v = h * dv_dx(v[n])
    
    k2_y = h * (v[n] + 0.5 * k1_v)
    k2_v = h * dv_dx(v[n] + 0.5 * k1_v)
    
    k3_y = h * (v[n] + 0.5 * k2_v)
    k3_v = h * dv_dx(v[n] + 0.5 * k2_v)
    
    k4_y = h * (v[n] + k3_v)
    k4_v = h * dv_dx(v[n] + k3_v)
    
    # Update y[n+1] and v[n+1]
    y[n+1] = y[n] + (1/6) * (k1_y + 2*k2_y + 2*k3_y + k4_y)
    v[n+1] = v[n] + (1/6) * (k1_v + 2*k2_v + 2*k3_v + k4_v)

# Plot the results
plt.figure(figsize=(10, 6))

# Plot y(x)
plt.subplot(2, 1, 1)
plt.plot(x, y, label='y(x)')
plt.title('Solution y(x) vs x')
plt.xlabel('x')
plt.ylabel('y(x)')
plt.grid(True)
plt.legend()

# Plot v(x) = dy/dx
plt.subplot(2, 1, 2)
plt.plot(x, v, label="v(x) = dy/dx", color='r')
plt.title('Solution v(x) = dy/dx vs x')
plt.xlabel('x')
plt.ylabel('v(x)')
plt.grid(True)
plt.legend()

# Show the plot
plt.tight_layout()
plt.show()

