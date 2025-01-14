import numpy as np
import cvxpy as cp

# Parabola parameters
V = np.array([[1, 0], [0, 0]])  # V matrix
u = np.array([-120, -0.5]).reshape(2, 1)  # u vector
f_scalar = 14400  # f scalar
x = cp.Variable((2, 1))  # Decision variable

# Cost function
e2 = np.array([0, 1]).reshape(1, 2)  # e2 vector
objective = cp.Minimize(e2 @ x)  # Objective function

# Constraints
constraint = [cp.quad_form(x, V) + 2 * u.T @ x + f_scalar <= 0]

# Define and solve the problem
problem = cp.Problem(objective, constraint)
result = problem.solve(solver=cp.SCS)  # Explicitly specify a solver

# Output the results
print("Optimal x:", x.value)

