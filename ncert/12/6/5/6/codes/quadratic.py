import numpy as np
from scipy.optimize import minimize

# Define the parameters
V = np.array([[-18, 0], [0, 0]])  # Quadratic term matrix
u = np.array([-36, -0.5])         # Linear term vector
f = 41                            # Constant term
e2 = np.array([0, 1])             # Objective coefficient

# Objective function
def objective(x):
    return -e2 @ x  # Maximize x2 (negative sign for maximization)

# Constraint function
def constraint(x):
    return x.T @ V @ x + 2 * u @ x + f

# Initial guess
x0 = np.zeros(2)

# Define the constraint
con = {'type': 'ineq', 'fun': constraint}  # Non-convex inequality

# Solve the problem
solution = minimize(objective, x0, constraints=[con])

# Output the results
print("Optimal x:", solution.x)
print("Optimal value:", -solution.fun)  # Negate to get the maximized value

