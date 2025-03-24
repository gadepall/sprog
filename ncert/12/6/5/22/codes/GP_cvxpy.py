import cvxpy as cp
import numpy as np

# Define the variable
x = cp.Variable(pos=True)

# Coefficients
a = np.pi**2 / 4 + np.pi
b = -7 * np.pi    
c = 49

# Define the objective function
objective = cp.Minimize(a * (x**2) + b * x + c)

# No additional constraints needed in this problem
constraints = []

# Define and solve the problem
problem = cp.Problem(objective, constraints)
problem.solve()

# Output the optimal value and the optimal solution
print(f"f_min: {problem.value}")
print(f"x_min: {x.value}")

