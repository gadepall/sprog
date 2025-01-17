import numpy as np
import matplotlib.pyplot as plt
import cvxpy as cp

# Define variables
x = cp.Variable(pos=True)  # x > 0
y = cp.Variable(pos=True)  # y > 0

# Define the constraints
constraints = [x + y == 60]  # Equality constraint

#Since xy^3 is non-convex, you can rewrite it using logarithms, as the logarithm of the objective will transform it into a concave form
# Define the objective function
objective = cp.Maximize(cp.log(x) + 3 * cp.log(y))  # Maximize log(x) + 3*log(y)

# Formulate the problem
problem = cp.Problem(objective, constraints)

# Solve the problem
problem.solve()

# Extract results
optimal_x = x.value
optimal_y = y.value
optimal_f = optimal_x * (optimal_y ** 3)  # f(x) = x * y^3

# Define the function and its derivative
def Z(y):
    return (60 * y**3) - y**4

def Z_prime(y):
    return (180 * y**2) - (4 * y**3)

# Gradient ascent parameters
h = 0.0001
epsilon = 1e-6
y_n = 20 # initial guess

# Gradient ascent loop
while True:
    y_next = y_n + h * Z_prime(y_n)
    if abs(y_next - y_n) < epsilon:
        break
    y_n = y_next

# Found maximum
max_y = y_n
max_Z = Z(max_y)

print("x value at maxima = ",60-max_y)
print("y value at maxima = ",max_y)
print("x using geometric programming" ,optimal_x)
print("y using geometric programming",optimal_y)
# Generate data for plotting
y_values = np.linspace(20, 50, 5000)
V_values = Z(y_values)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(y_values, V_values, label="Curve", color="blue")
plt.scatter(max_y, max_Z, color="red", label=f"Maxima at y={max_y:.2f}")
plt.xlabel("y")
plt.ylabel("Z(y)")
plt.legend()
plt.grid()
plt.show()
