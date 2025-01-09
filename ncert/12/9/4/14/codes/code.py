import matplotlib.pyplot as plt
import numpy as np
from ctypes import *

# Load the C library
func = CDLL('./code.so')

# Define the functions from the library
x_n = func.x_n
y_n = func.y_n

# Set return types for the C functions
x_n.restype = c_double
y_n.restype = c_double

# Parameters
h = 0.01  # Step size
x_min = 0.1  # To avoid singularity at x=0
x_max = 1
n_steps = int((x_max - x_min) / h) + 1

# Theoretical Solution
x_theory = np.linspace(x_min, x_max, 500)
y_theory = x_theory * np.arccos(x_theory)  # y = x * arccos(x)

# Computational Solution using the C library
x_comp = [1.0]  # Initial x0
y_comp = [0.0]  # Initial y0

for i in range(n_steps):
    x_prev = x_comp[-1]
    y_prev = y_comp[-1]
    
    # Compute the next x and y values using the C library functions
    x_next = x_n(c_double(x_prev), c_double(h))
    y_next = y_n(c_double(x_prev), c_double(y_prev), c_double(h))
    
    # Avoid invalid values
    if x_next <= 0 or np.isnan(x_next) or np.isnan(y_next):
        break
    
    x_comp.append(x_next)
    y_comp.append(y_next)

# Convert lists to arrays
x_comp = np.array(x_comp)
y_comp = np.array(y_comp)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(x_theory, y_theory, label='Theoretical Solution', color='blue', lw=2)
plt.plot(x_comp, y_comp, label='Computational Solution', color='orange', linestyle='--', lw=2)
plt.xlabel('x', fontsize=14)
plt.ylabel('y', fontsize=14)
plt.title('Solutions of the Differential Equation', fontsize=16)
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')
plt.grid(alpha=0.3)
plt.legend(fontsize=12)
plt.show()

