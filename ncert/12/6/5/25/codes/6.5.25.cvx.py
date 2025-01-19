import cvxpy as cp
import numpy as np

# Parameters
pi = np.pi
l = 10  # cm (fixed constant)

# Variable
r = cp.Variable(nonneg=True)  # radius r

# Objective function: log(V) = 2*log(r) + (1/2)*log(l^2 - r^2)
objective = cp.Maximize(2 * cp.log(r) + 0.5 * cp.log(l**2 - r**2))

# Constraints: r must be between 0 and l
constraints = [r >= 0, r <= l]

# Problem formulation
problem = cp.Problem(objective, constraints)

# Solve the problem
problem.solve()

# Results
optimal_r = r.value
optimal_V = (1/3) * pi * optimal_r**2 * np.sqrt(l**2 - optimal_r**2)

# Calculate the value of r / sqrt(l^2 - r^2) at the maximum volume
value_r_over_sqrt = optimal_r / np.sqrt(l**2 - optimal_r**2)

print(f"Optimal r: {optimal_r:.2f} cm")
print(f"Maximum Volume V: {optimal_V:.2f} cm^3")
print(f"Value of r / sqrt(l^2 - r^2): {value_r_over_sqrt:.2f}")
