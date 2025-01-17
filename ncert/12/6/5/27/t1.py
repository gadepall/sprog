import numpy as np
from cvxpy import *

# Define the variable
x = Variable(2)  # We have two variables: x[0] and x[1]

# Define the cost function: squared Euclidean distance to point P = (0, 5)
P = np.array([0, 5])  # Target point
f = (x[0] - P[0])**2 + (x[1] - P[1])**2  # Squared Euclidean distance to the point P

# Define the constraint for the parabola: x^2 = 2y
# Instead of x[0]^2 = 2 * x[1], we use a convex reformulation.
constraints = [x[0]**2 - 2 * x[1] >= 0]

# Define the optimization problem
obj = Minimize(f)
problem = Problem(obj, constraints)

# Solve the problem
problem.solve()

# Output the results
print("Optimal distance:", np.sqrt(f.value))
print("Optimal point:", x.value)

