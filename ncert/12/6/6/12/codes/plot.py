#Code by S.Sai Akshita
#importing necessary libraries for plotting
import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# Defining the function for theoretical plot
def func(theta, a, b):
    return a / np.cos(theta) + b / np.sin(theta)

# Defining Parameters 
a = 2.0  # Example value for a
b = 3.0  # Example value for b
theta_0 = 0.103  # Initial value of theta (random starting point)
c_0 = func(theta_0, a, b)  # Initial value of c for theta_0
eta = 0.0001  # Step size for gradient descent
epsilon = 0.000001  # Convergence threshold
max_iterations = 3000 # Maximum iterations to avoid infinite loops

#Calling C Function
function = ctypes.CDLL('./gradient.so')
function.nextstep.argtypes = [ctypes.c_double]
function.nextstep.restype = ctypes.c_double

#Theta values for theoretical plot (avoiding 0 and pi to prevent division by zero)
theta_vals = np.linspace(0.1, np.pi/2 - 0.1, 500)

#Corresponding c values for theoretical plot
c_vals = func(theta_vals, a, b)

# Set up the gradient descent simulation
theta = [theta_0]
c = [c_0]
iteration = 0

# Gradient descent loop for simulation
while iteration < max_iterations:
    current_theta = theta[-1]
    
    # Call the C function to get the next step
    new_theta = function.nextstep(ctypes.c_double(current_theta))

    # If the change is very small, stop the iteration
    if abs(new_theta - current_theta) < epsilon:
        break

    # Append the updated values to the lists
    theta.append(new_theta)
    c.append(func(new_theta, a, b))  # Recalculate c for the new theta value
    iteration += 1

# Convert lists to numpy arrays for plotting
theta = np.array(theta)
c = np.array(c)
print(theta[-1],c[-1])

# Plotting theoretical and simulation results
plt.figure(figsize=(8, 6))
plt.plot(theta_vals, c_vals, label='Theory', linewidth=5, color='green')
plt.plot(theta, c, label='Sim', color='yellow')
plt.plot(theta[-1],c[-1], 'ro',label="local minima")
plt.title(r'Plot of $c = a \sec(\theta) + b \csc(\theta)$')
plt.xlabel(r'$\theta$ (radians)')
plt.ylabel(r'$c$')
plt.legend()
plt.grid(True)
plt.savefig('Gradient.jpeg')
plt.show()

