import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
doolittle_solver = ctypes.CDLL('./doolittle_solver.so')

# Define function argument types
doolittle_solver.solve_and_generate.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # F
    ctypes.POINTER(ctypes.c_double),  # C
    ctypes.POINTER((ctypes.c_double * 2) * 10),  # Points for eq1
    ctypes.POINTER((ctypes.c_double * 2) * 10),  # Points for eq2
]

# Prepare variables
F = ctypes.c_double()
C = ctypes.c_double()
points1 = ((ctypes.c_double * 2) * 10)()
points2 = ((ctypes.c_double * 2) * 10)()

# Call the C function
doolittle_solver.solve_and_generate(ctypes.byref(F), ctypes.byref(C), points1, points2)

# Convert to NumPy arrays for plotting
points1 = np.array([[p[0], p[1]] for p in points1])
points2 = np.array([[p[0], p[1]] for p in points2])

# Extend the lines for clarity
F_vals = np.linspace(0, 20, 100)
C1_vals = (27 - F_vals) / 4
C2_vals = (21 - F_vals) / 2

# Plot the extended lines
plt.plot(F_vals, C1_vals, 'r-', label="x + 4y = 27")
plt.plot(F_vals, C2_vals, 'b-', label="x + 2y = 21")

# Mark the intersection point
plt.scatter([F.value], [C.value], color="black", zorder=3, label=f"Solution ({F.value}, {C.value})")

# Adjust axis limits
plt.xlim(0, 20)
plt.ylim(0, 10)

# Labels and legend
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.savefig('./figs/curve.png')

