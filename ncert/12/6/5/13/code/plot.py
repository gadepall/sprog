import ctypes
import numpy as np
import cvxpy as cp
import matplotlib.pyplot as plt

# Load the shared library
gradient_ascent = ctypes.CDLL('./points.so')

# Define argument and return types for the C gradient ascent function
gradient_ascent.gradient_ascent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
gradient_ascent.gradient_ascent.restype = ctypes.c_double

# Call the gradient ascent function
start_x = 0.0
learning_rate = 0.1
max_iterations = 1000
maxima_ga = gradient_ascent.gradient_ascent(start_x, learning_rate, max_iterations)
maxima_ga_value = 24 * maxima_ga - maxima_ga**2

# Solve optimization problem using CVXPY (Geometric Programming)
x_gp = cp.Variable(pos=True)  # Positive variable for GP
objective_gp = cp.Maximize(24 * x_gp - x_gp**2)  # Maximize quadratic function
problem_gp = cp.Problem(objective_gp)
result_gp = problem_gp.solve()

maxima_gp = x_gp.value  # Optimal x from GP
maxima_gp_value = result_gp  # Optimal value from GP

# Print results
print("Gradient Ascent:")
print(f"  Maxima x = {maxima_ga:.4f}, Function value = {maxima_ga_value:.4f}")
print("Geometric Programming:")
print(f"  Maxima x = {maxima_gp:.4f}, Function value = {maxima_gp_value:.4f}")

# Define the quadratic function for plotting
x = np.linspace(0, 30, 500)  # Range for x
y = 24 * x - x**2  # Quadratic function

# Plot the function and the maxima
plt.plot(x, y, label="curve: 24x - x^2", color='blue')
plt.scatter(maxima_ga, maxima_ga_value, color='red', label=f"sim1: Gradient Ascent (x={maxima_ga:.2f})")
plt.scatter(maxima_gp, maxima_gp_value, color='green', label=f"sim2: Geometric Programming (x={maxima_gp:.2f})")
plt.title("Function and Maxima Points")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.show()

