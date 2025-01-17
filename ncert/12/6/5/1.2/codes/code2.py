import numpy as np
from scipy.optimize import minimize

# Define the parameters
V = np.array([[9, 0], [0, 0]])  # Quadratic term matrix
u = np.array([6, 0]).reshape(-1, 1)  # Linear term vector
f = 2  # Constant term

# Objective function: Quadratic function with column matrix X
def objective(X_flat):
    X = X_flat.reshape(-1, 1)
    # Calculate the quadratic function: X.T @ V @ X + 2 * u.T @ X + f
    quad_term = (X.T @ V @ X).item()  
    linear_term = (2 * u.T @ X).item()
    return quad_term + linear_term + f

# Initial guess: Start with a zero column matrix (2x1)
X0 = np.zeros((2, 1)).flatten()

# Solve the optimization problem
solution = minimize(objective, X0)

# Reshape the solution back to a column matrix (2x1)
optimal_X = solution.x.reshape(-1, 1)

# Output the results
print("Optimal X value :")
print(optimal_X[0])
print("Optimal value of the quadratic function (minimum value):", solution.fun)

