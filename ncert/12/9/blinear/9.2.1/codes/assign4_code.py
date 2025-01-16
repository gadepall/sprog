import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the C shared object file
lib = ctypes.CDLL('./libdiff_eq_solver.so')

# Define the number of iterations and step size
N = 1000
h = 0.0001  # Reduce step size for better accuracy

# Create arrays for the points
y = np.zeros(N)
v = np.zeros(N)
y_trap = np.zeros(N)
v_trap = np.zeros(N)
y_bilin = np.zeros(N)
v_bilin = np.zeros(N)

# Define the argument types of the C function
lib.compute_points.argtypes = [
    ctypes.c_double,  # h (step size)
    ctypes.POINTER(ctypes.c_double),  # y array
    ctypes.POINTER(ctypes.c_double),  # v array
    ctypes.POINTER(ctypes.c_double),  # y_trap array
    ctypes.POINTER(ctypes.c_double),  # v_trap array
    ctypes.POINTER(ctypes.c_double),  # y_bilin array
    ctypes.POINTER(ctypes.c_double)   # v_bilin array
]

# Call the C function
lib.compute_points(h, 
                   y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   v.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   y_trap.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   v_trap.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   y_bilin.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                   v_bilin.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))

# Theoretical solution y = e^x + 1
x_values = np.linspace(0, (N-1)*h, N)
y_theory = np.exp(x_values) + 1

# Plotting the results
plt.figure(figsize=(10, 6))

# Plot Theory curve with a bright blue color and a thicker line
plt.plot(x_values, y_theory, label="Theory", color='deepskyblue', linestyle='-', linewidth=3)

# Plot Trapezoidal simulation curve with a bright green color and thicker line
plt.plot(x_values, y_trap, label="Sim1 (Trapezoidal)", color='limegreen', linestyle='--', linewidth=3)

# Plot Bilinear Transform simulation curve with a bright red color and thicker line
plt.plot(x_values, y_bilin, label="Sim2 (Bilinear Transform)", color='tomato', linestyle=':', linewidth=3)

# Adding labels and legend with larger font sizes
plt.xlabel('x', fontsize=14)
plt.ylabel('y(x)', fontsize=14)

plt.legend(loc='upper left', fontsize=12)
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.savefig('./figs/curve.png')
# Show plot with improved visibility
plt.show()



