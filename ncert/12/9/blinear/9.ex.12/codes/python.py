import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
solver = ctypes.CDLL('./trapezoidal_solver.so')

# Specify argument types for the C function
solver.solve_trapezoidal.argtypes = [
    ctypes.c_double,  # x0
    ctypes.c_double,  # y0
    ctypes.c_double,  # h
    ctypes.c_int,     # steps
    ctypes.POINTER(ctypes.c_double)  # results array
]

# Parameters
x0 = 1.0
y0 = 1.0
h = 0.001
steps = 5000  # Number of steps for all methods

# Prepare array for C results
results = (ctypes.c_double * steps)()
solver.solve_trapezoidal(x0, y0, h, steps, results)

# Convert C results to NumPy array for plotting
y_trapezoidal = np.array(results)
x_values = np.linspace(x0, x0 + h * (steps - 1), steps)

# Analytical solution: y = x^2 + log|x|
y_analytical = x_values**2 + np.log(np.abs(x_values))

# Python Numerical Method (from your image)
t_python = [x0, x0 + h]  # Initial x values
y_python = [y0, y0 + h]  # Initial y values

# Implementing the numerical method
for i in range(2, steps):
    new_t = t_python[-1] + h  # Increment x
    f_n = 2 * t_python[-1] + (1 / t_python[-1])  # f(x[n])
    f_n1 = 2 * new_t + (1 / new_t)               # f(x[n+1])
    new_y = y_python[-1] + (h / 2) * (f_n + f_n1)  # Trapezoidal update
    t_python.append(new_t)
    y_python.append(new_y)

# Convert Python numerical results to NumPy arrays for plotting
t_python = np.array(t_python)
y_python = np.array(y_python)

# === Plot All Solutions on the Same Graph ===
plt.figure(figsize=(10, 6))

# C Trapezoidal Approximation
plt.plot(x_values, y_trapezoidal, marker='o', linestyle='--', label="Sim1", markersize=3)

# Analytical Solution
plt.plot(x_values, y_analytical, label=r"Theoretical", color='red')

# Python Numerical Solution
plt.plot(t_python, y_python, linestyle='dotted', linewidth=1.5, label="Sim2")

# Plot Formatting
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)

# Save and Show the Plot
plt.savefig('fig.pdf')
plt.show()

