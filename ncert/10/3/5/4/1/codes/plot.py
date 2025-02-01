import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object
lu_solver = ctypes.CDLL("./lu_solver.so")

# Prepare the result array
result = (ctypes.c_double * 2)()

# Solve the system
lu_solver.solve(result)

# Extract solution
x, y = result[0], result[1]
print(f"Solution: x = {x}, y = {y}")

# Plot for positive axes only
y_vals = np.linspace(0, 100, 100)
x1_vals = 1000 - 20 * y_vals
x2_vals = 1080 - 26 * y_vals

# Filter positive values
valid_x1 = x1_vals >= 0
valid_x2 = x2_vals >= 0

plt.figure(figsize=(8, 6))
plt.plot(y_vals[valid_x1], x1_vals[valid_x1], label="x + 20y = 1000")
plt.plot(y_vals[valid_x2], x2_vals[valid_x2], label="x + 26y = 1080")

if x >= 0 and y >= 0:
    plt.scatter(y, x, color="red", label=f"Solution ({x:.2f}, {y:.2f})")

plt.xlim(0, max(y_vals))
plt.ylim(0, max(max(x1_vals), max(x2_vals)))

plt.xlabel("y")
plt.ylabel("x")
plt.title("LU Decomposition Solution (Positive Axes)")
plt.legend()
plt.grid()
plt.show()

