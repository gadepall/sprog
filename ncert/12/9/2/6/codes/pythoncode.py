import numpy as np
import matplotlib.pyplot as plt

# Define the theoretical solution
def theoretical_solution(x):
    return x * np.sin(x)

# Define the differential equation's derivative function
def dydx(x, y):
    return y / x + np.sqrt(x**2 - y**2)

# Numerical solution using Euler's method
def euler_method(x0, y0, h, x_end):
    x_vals = [x0]
    y_vals = [y0]
    
    while x_vals[-1] < x_end:
        x_n = x_vals[-1]
        y_n = y_vals[-1]
        y_next = y_n + h * dydx(x_n, y_n)
        x_next = x_n + h
        x_vals.append(x_next)
        y_vals.append(y_next)
    
    return np.array(x_vals), np.array(y_vals)

# Parameters
x0 = 0.1  # Initial x (avoid x=0 to prevent division by zero)
y0 = theoretical_solution(x0)  # Initial y from the theoretical solution
h = 0.05  # Step size
x_end = 2 # End of x range

# Compute theoretical and numerical solutions
x_theoretical = np.linspace(x0, x_end, 500)
y_theoretical = theoretical_solution(x_theoretical)
x_numerical, y_numerical = euler_method(x0, y0, h, x_end)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(
    x_theoretical, 
    y_theoretical, 
    label="Theory", 
    color="blue"
)
plt.plot(
    x_numerical, 
    y_numerical, 
    linestyle=':',  # Use only a dotted line
    label="Sim", 
    color="red"
)
plt.xlabel("$x$", fontsize=14)
plt.ylabel("$y$", fontsize=14)

plt.legend(fontsize=12)
plt.grid(True)
plt.show()

