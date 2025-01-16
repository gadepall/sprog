import numpy as np 
from scipy.optimize import minimize 

#define the function (objective)
def objective_function(x):
    a = np.pi**2/4 + np.pi
    b = -7 * np.pi
    c = 49
    return a * x * x + b * x + c

#initial guess
x0 = [0] 

#use scipy to minimize it
result = minimize(objective_function, x0 , method = 'BFGS')

print("Optimization Result:")
print(f"Minimum value of the function: {result.fun}")
print(f"Value of x at the minimum: {result.x[0]}")
print(f"Success: {result.success}")
print(f"Message: {result.message}")
