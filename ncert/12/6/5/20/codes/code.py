import ctypes
import math
import matplotlib.pyplot as plt
import cvxpy as cp
import numpy as np

# Define the Result structure to match the C struct
class Result(ctypes.Structure):
    _fields_ = [("r_opt", ctypes.c_double),
                ("h_opt", ctypes.c_double)]

# Load the shared library
lib = ctypes.CDLL('./lib.so')

# Define the argument and return types of the C functions
lib.gradient.argtypes = [ctypes.c_double, ctypes.c_double]
lib.gradient.restype = ctypes.c_double

# Correcting restype to directly return Result instead of a pointer
lib.gradient_ascent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.gradient_ascent.restype = Result

def gradient(r, S):
    return lib.gradient(r, S)

def gradient_ascent(S, lr, tol, itr):
    # Directly return the result structure instead of dereferencing a pointer
    result = lib.gradient_ascent(S, lr, tol, itr)
    return result

# Function to calculate volume v as a function of r
def calculate_volume(S, r):
    return (r * (S - 2 * math.pi * r**2)) / 2

# Function to plot v vs r
def plot_v_vs_r(S, lr, tol, itr):
    r_values = []  # Store the r_opt values
    v_values = []  # Store the corresponding volume values
    
    # Generate r values and calculate corresponding volumes
    r_min = 0.1  # Start radius value
    r_max = 12.0  # End radius value
    r_step = 0.1  # Step size for radius values
    
    r = r_min
    while r <= r_max:
        v = calculate_volume(S, r)  # Calculate volume for this radius
        r_values.append(r)
        v_values.append(v)
        r += r_step
    
    result = gradient_ascent(S, lr, tol, itr)
    r_opt = result.r_opt
    v_opt = calculate_volume(S, r_opt)
    
    # Plot the results
    plt.plot(r_values, v_values, marker='o', linestyle='-', color='g', label="v vs r")
    plt.scatter(r_opt, v_opt, color='red', zorder=5, label="Max point")
    plt.scatter(r_gp, v_gp, color='yellow', zorder=5, label="Geometric Programming")
    plt.xlabel("Radius (r)")
    plt.ylabel("Volume (v)")
    plt.title("Volume (v) vs Radius (r)")
    plt.grid(True)
    plt.legend()
    plt.savefig('../figs/fig.png')
    plt.show()

# Example usage:
S = 1000.0  # Surface area
lr = 0.0001  # Learning rate (smaller value for better convergence)
tol = 1e-10  # Tolerance (more precise)
itr = 2000 # Number of iterations (increased)

# Get the result from gradient ascent
result = gradient_ascent(S, lr, tol, itr)

# Define variables
r = cp.Variable(pos=True)  # Radius (must be positive)
h = cp.Variable(pos=True)  # Height (must be positive)

# Define objective: Maximize volume V = π r² h
objective = cp.Maximize(np.pi * r**2 * h)

# Define constraint: Surface area constraint
constraint = [(4*np.pi*np.pi*r*r)*((r + h)**2) <= S**2]
# Define the problem as a geometric program
problem = cp.Problem(objective, constraint)

# Solve the problem
problem.solve(gp=True, solver = cp.MOSEK)

# Extract optimal values
r_gp = r.value
h_gp = h.value
v_gp = calculate_volume(S, r_gp)

# Print the optimal radius for maximum volume
print(f"Optimal radius for maximum volume: {result.r_opt:.8f}")
print(f"Optimal radius (r): {r_gp}")
print(f"Optimal height (h): {h_gp}")
print(f"h / r = {h_gp/r_gp}")
# Call the function to plot v vs r
plot_v_vs_r(S, lr, tol, itr)
