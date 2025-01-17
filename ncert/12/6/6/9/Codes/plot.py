import numpy as np
import matplotlib.pyplot as plt
import ctypes
import cvxpy as cp

# Load the shared library
lib = ctypes.CDLL('./lib.so')

lib.function.argtypes=[ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
lib.derivative.argtypes=[ctypes.c_double]
lib.derivative.restype=ctypes.c_double
lib.gradient_descent.restype=ctypes.c_double

# Parameters
x_start = 0
x_end = 5
h = 0.01
n_steps = 501

# Setting up the array
x = np.linspace(x_start, x_end, n_steps)
y = np.zeros(n_steps)

# Conversion to ctypes array
x_ctypes = x.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_ctypes = y.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the C functions
lib.function(x_ctypes, y_ctypes, n_steps)
min_point = lib.gradient_descent()
min_value = 280 + 180 * min_point + 720 / min_point
print(f"Minimum found at x = {min_point}, f(x) = {min_value}")

# Plotting
plt.figure(figsize=(10, 6))

plt.plot(x, y, label="Theory", linestyle='-', color='b', linewidth=5)
plt.scatter(min_point, min_value, color='r', marker='o', s=100, zorder=5, label=f"Min at x={min_point:.2f}")
plt.xlabel("x")
plt.ylabel("y")
plt.legend(['f(x)=280+180x+720/x', f'Min at x={min_point:.2f}'])
plt.grid()
plt.savefig('plot.png')

#Geometric programming 

#Define the variables
x=cp.Variable(pos=True)

#Define the objective function
objective=cp.Minimize(280+180*x+720/x)

#Defining the problem 
problem=cp.Problem(objective)

#Solving the problem with geometric programming
problem.solve(gp=True)
print("Results obtained from geometric programming")
print("Minimized x value: ",x.value)
print("Minimized cost value: ",problem.value)

#Quadratic programming approach by treating 1/x as y 
# Define variables
x_1 = cp.Variable(pos=True)  # x > 0
y_1 = cp.Variable(pos=True)  # y > 0

# Define constraints
constraints_1 = [y_1 == 1/x_1] #(for quadratic programming the constraints must be linear but cvxpy supports non linear also)

# Define the objective function
objective_1 = cp.Minimize(280 + 180 * x_1 + 720 * y_1)

# Formulate and solve the problem
problem_1 = cp.Problem(objective_1, constraints_1)
problem_1.solve()

# Print the results
print("Optimal value of x:", x_1.value)
print("Optimal value of y:", y_1.value)
print("Minimum value of the function:", problem_1.value)



