import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lu_solver = ctypes.CDLL("./lu_solver.so")

# Define return type and argument types
lu_solver.solve_system.argtypes = [ctypes.POINTER(ctypes.c_double)]
lu_solver.solve_system.restype = None

# Prepare output array
x_out = (ctypes.c_double * 2)()
lu_solver.solve_system(x_out)

# Extract solutions
x1, x2 = x_out[0], x_out[1]
print(f"Solution: x1 = {x1}, x2 = {x2}")

# Corrected equations
x_vals = np.linspace(-5, 5, 100)
y1_vals = x_vals + 2  # Correct: y = x + 2 (Blue)
y2_vals = 2 * x_vals - 1  # Correct: y = 2x - 1 (Red)

# Plot
plt.plot(x_vals, y1_vals, label="x - y = -2", color="blue")  # Fixed
plt.plot(x_vals, y2_vals, label="2x - y = 1", color="red")
plt.scatter([x1], [x2], color="black", zorder=3, label="Intersection")

plt.xlabel("x1")
plt.ylabel("x2")
plt.axhline(0, color="black", linewidth=0.5)
plt.axvline(0, color="black", linewidth=0.5)
plt.legend()
plt.grid()
plt.title("Graph of Equations and Intersection Point")
plt.show()

