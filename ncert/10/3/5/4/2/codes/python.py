import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object
lu_solver = ctypes.CDLL("./ccode.so")

# Prepare the result array
result = (ctypes.c_double * 2)()

# Solve the system using LU decomposition
lu_solver.solve(result)

# Extract solution from the result
x, y = result[0], result[1]
print(f"Solution: x = {x}, y = {y}")

# Define the equations
def equation_1(y):
    return 1 + y / 3  # x = 1 + y/3

def equation_2(y):
    return (y + 8) / 4  # x = (y + 8)/4

# Define y values for the plot
y_vals = np.linspace(0, 20, 100)

# Calculate x values for both equations
x1_vals = equation_1(y_vals)
x2_vals = equation_2(y_vals)

# Plot the two lines
plt.figure(figsize=(8, 6))
plt.plot(x1_vals, y_vals, label="(x - 1) / y = 1/3", color="blue")
plt.plot(x2_vals, y_vals, label="x / (y + 8) = 1/4", color="green")

# Highlight the intersection point
plt.scatter(x, y, color="red", label=f"Intersection ({x:.2f}, {y:.2f})", zorder=5)

# Labels and grid
plt.xlabel("x")
plt.ylabel("y")

plt.legend()
plt.grid(True)

# Show the plot
plt.show()
