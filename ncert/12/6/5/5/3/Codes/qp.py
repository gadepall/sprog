import cvxpy as cp

# Define the variable
x = cp.Variable()

# Define the quadratic objective function for maximization
objective_max = cp.Maximize(4 * x - (x**2) / 2)




# Define the constraints
constraints = [x >= -2, x <= 4.5]

# Define and solve the maximization problem
problem_max = cp.Problem(objective_max, constraints)
problem_max.solve()

# Define and solve the minimization problem



# Extract the results
optimal_x_max = x.value
optimal_f_max = 4 * optimal_x_max - (optimal_x_max**2) / 2



# Print results
print(f"Optimal x (maximum): {optimal_x_max}")
print(f"Optimal f(x) (maximum): {optimal_f_max}")

objective_min = cp.Minimize(4 * x - (x**2) / 2)
problem_min = cp.Problem(objective_min, constraints)
problem_min.solve()
optimal_x_min = x.value
optimal_f_min = 4 * optimal_x_min - (optimal_x_min**2) / 2
print(f"Optimal x (minimum): {optimal_x_min}")
print(f"Optimal f(x) (minimum): {optimal_f_min}")

