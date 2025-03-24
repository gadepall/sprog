import ctypes
import numpy as np
import matplotlib.pyplot as plt
import math
import cvxpy as cp
import os

# Load the shared library
gen = ctypes.CDLL('./code.so')

# Define argument and return types for the functions
gen.generate_points.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
gen.generate_points.restype = None

FUNC_TYPE = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
gen.run_gradient_descent.argtypes = [
    ctypes.c_double, ctypes.c_double, FUNC_TYPE,
    ctypes.c_double, ctypes.POINTER(ctypes.c_double)
]
gen.run_gradient_descent.restype = None

# Parameters
init_guess = 1.0  # Initial guess for 'r'
step_size = 0.01  # Learning rate
min_value = ctypes.c_double(0.0)
num_points = 10000
tolerance = 1e-8  # Increased precision for tolerance
pi = math.pi
Vol = 1  # Unit volume

# Create a numpy array for the points
points = np.zeros((num_points, 2), dtype=np.double)

# Define the derivative function (CSA formula derivative)
def f_derivative(r):
    # Compute the height from the volume constraint
    h = (3 * Vol) / (pi * r**2)
    
    # Terms in the CSA derivative
    term1 = 2 * r  # Derivative of r^2
    term2 = -2 * h * ((6 * Vol) / (pi * r**3))  # Derivative of (3*Vol)/(pi*r^2)
    
    f_r = r**2 + h**2  # r^2 + h^2
    f_rder = term1 + term2  # Derivative of f(r)
    
    # Gradient of CSA
    return pi * (math.sqrt(f_r) + (r / (2 * math.sqrt(f_r))) * f_rder)

# Call the generate_points function
gen.generate_points(
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Reshape the points array for easier plotting
points = points.reshape((num_points, 2))

# Call the run_gradient_descent function
gen.run_gradient_descent(
    init_guess, step_size, FUNC_TYPE(f_derivative), tolerance, ctypes.byref(min_value)
)

# Compute height and h/r
converged_r = min_value.value
converged_h = (3 * Vol) / (pi * converged_r**2)
h_by_r = converged_h / converged_r

# Print the result
print(f"Converged value of 'r': {converged_r}")
print(f"The value of 'h': {converged_h}")
print(f"The value of 'h/r': {h_by_r}")

# Generate data for CSA vs h/r plot
r_values = np.linspace(0.01, 100, num_points)
h_values = (3 * Vol) / (pi * r_values**2)
h_by_r_values = h_values / r_values
csa_values = pi * r_values * np.sqrt(r_values**2 + h_values**2)

# Perform Geometric Programming
r_gp = cp.Variable(pos=True)  # Radius (r) must be positive

# Define the objective for CSA
objective = cp.Minimize(pi * r_gp * cp.sqrt(r_gp**2 + (3 * Vol / (pi * r_gp**2))**2))

# Solve the geometric programming problem
problem = cp.Problem(objective)
problem.solve(gp=True)  # Solve as a geometric program

# Extract results from geometric programming
optimal_r_gp = r_gp.value
optimal_h_gp = (3 * Vol) / (pi * optimal_r_gp**2)
optimal_csa_gp = pi * optimal_r_gp * math.sqrt(optimal_r_gp**2 + optimal_h_gp**2)
optimal_h_by_r_gp = optimal_h_gp / optimal_r_gp

# Print results from geometric programming
print("Results obtained by Geometric Programming:")
print(f"Optimal radius (r): {optimal_r_gp}")
print(f"Optimal CSA: {optimal_csa_gp}")
print(f"Optimal h/r: {optimal_h_by_r_gp}")

# Plot CSA vs h/r
fig, ax1 = plt.subplots()
ax1.plot(h_by_r_values, csa_values, c='r', label='CSA vs h/r')
ax1.scatter(h_by_r, pi * converged_r * np.sqrt(converged_r**2 + converged_h**2),
            c='b', label='Gradient Descent Optimal Point')
ax1.scatter(optimal_h_by_r_gp, optimal_csa_gp, color='yellow', label='Geometric Programming Optimal Point')

# Set plot limits, labels, and legend
ax1.set_xlim(0, 30)
ax1.set_ylim(0, 30)
ax1.set_xlabel('h/r')
ax1.set_ylabel('CSA')
ax1.legend(loc="best")

# Save the plot
plt.savefig("../figures/fig.png")
plt.show()

