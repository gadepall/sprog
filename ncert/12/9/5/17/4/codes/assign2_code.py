import ctypes
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

# Load the shared object
lib = ctypes.CDLL('./points.so')

# Define argument and return types for the C function
lib.generate_points.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.c_double, ctypes.c_double]
lib.generate_points.restype = ctypes.POINTER(ctypes.c_double)

# Parameters
h = 0.001
x0, y0 = 0.001, 1.0  # Avoid x=0 to prevent division by zero
n_points = 10000

# Call the C function
data_ptr = lib.generate_points(n_points, x0, y0, h)

# Convert the pointer to a NumPy array
data = np.ctypeslib.as_array(data_ptr, shape=(2 * n_points,))
x_sim, y_sim = data[0::2], data[1::2]

# Define the theoretical solution
def dydx(x, y):
    denom = x ** 2 - x * y - y ** 2
    if denom == 0:
        return 0
    return -y ** 2 / denom

x_theory = np.linspace(x0, x0 + n_points * h, n_points)
sol = solve_ivp(dydx, [x_theory[0], x_theory[-1]], [y0], t_eval=x_theory)

# Plot both curves
plt.figure(figsize=(10, 6))
plt.plot(x_sim, y_sim, 'b--', label="Sim", alpha=0.8)
plt.plot(sol.t, sol.y[0], 'r-', label="Theory", alpha=0.8)
plt.scatter(x_sim[::100], y_sim[::100], color='blue', s=10, label="Sim Points (Markers)")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.savefig('./figs/curve.png')

