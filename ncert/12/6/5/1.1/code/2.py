import cvxpy as cp

# Define the variable
x = cp.Variable()

# Define the objective function
objective = cp.Minimize((2 * x - 1)**2 + 3)

# Define the problem
problem = cp.Problem(objective)

# Solve the problem
problem.solve()

# Print the results
print("Minimum value:", problem.value)
print("x value:", x.value)

