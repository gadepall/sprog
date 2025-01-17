import cvxpy as cp

# Define the optimization variable
x = cp.Variable()

# Define the quadratic programming problem
Q = 2  # Quadratic term
c = -2  # Linear term

# Objective function
objective = cp.Minimize(0.5 * Q * cp.square(x) + c * x)

# Solve the problem
problem = cp.Problem(objective)
problem.solve()

# Display the result
optimal_x = x.value
optimal_y = -(optimal_x - 1)**2 + 10

print(f"The value of x that maximizes the function is: {optimal_x}")
print(f"The maximum value of the function is: {optimal_y}")
