import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared library
solver = ctypes.CDLL('./solver.so')

# Set argument and return types for the C functions
solver.finite_differences.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_double]
solver.analytical_solution.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]

# Parameters
x_start = 0
x_end = 5
h = 0.1
n_steps = 51

# Initialize x and y arrays
x = np.linspace(x_start, x_end, n_steps)
y_fd = np.zeros(n_steps)
y_exact = np.zeros(n_steps)
x_z=[0.0]
y_z=[0.0]
for i in range(52):
    x_z.append(x_z[i]+h)
    y_z.append(h*((1-h))**i)

# Convert arrays to ctypes
x_ctypes = x.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_fd_ctypes = y_fd.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_exact_ctypes = y_exact.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the C functions
solver.finite_differences(x_ctypes, y_fd_ctypes, n_steps, h)
solver.analytical_solution(x_ctypes, y_exact_ctypes, n_steps)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(x, y_fd, label="Simulation", linestyle='--', color='b')
plt.plot(x, y_exact, label="Theory", linestyle='-', color='r')
plt.plot(x_z, y_z, label="Simulation 2", linestyle='--', color='g')

plt.xlabel("x")
plt.ylabel("y")
#plt.legend()
plt.legend(['Sim1', 'Theory','Sim2(Z Transform)'])
plt.grid()
#plt.show()
plt.savefig('plot.png')
