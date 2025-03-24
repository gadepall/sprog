import numpy as np
from scipy.optimize import minimize

# Define the dimension
n = 2  # 2-dimensional problem

# Define the parameters
e2 = np.array([0, 1])  # e2 is [0, 1] in 2 dimensions
V = np.array([[1, 0], [0, 0]])  # Example V
u = np.array([0, -0.5])         # Example u
f = 0                           # Example f

# Define the objective function
def objective(x):
    return e2.T @ x

# Define the constraint function
def constraint(x):
    return np.dot(x.T, np.dot(V, x)) + 2 * np.dot(u.T, x) + f

# Initial guess
x0 = np.zeros(n)

# Define the constraints in the form required by `scipy.optimize`
con = {'type': 'eq', 'fun': constraint}

# Solve the problem
solution = minimize(objective, x0, constraints=[con])

# Output the results
print("Optimal value:", solution.fun)
print("Optimal x:", solution.x)

