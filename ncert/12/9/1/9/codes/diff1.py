import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, POINTER, c_double, c_int

# Load the shared C library that contains the ODE solver function
ode_solver = CDLL("./ode_solver.so")

# Define initial conditions and parameters
y0 = 0.1      # Initial value of the dependent variable y(x)
y_prime0 = -0.1  # Initial value of the first derivative of y, i.e., y'(x)
x_max = 10    # The maximum value of the independent variable (x) (end of range)
dx = 0.001    # The step size for the independent variable (x)

# Define the number of steps to compute based on the range and step size
num_steps = int(x_max / dx) + 1

# Create an array to store the computed values of y(x) at each step
y_values = np.zeros(num_steps, dtype=np.float64)

# Prepare the input arguments for the C function
x_values_ctypes = np.arange(0, x_max + dx, dx).ctypes.data_as(POINTER(c_double))
y_values_ctypes = y_values.ctypes.data_as(POINTER(c_double))

# Call the ODE solver function from the C library to compute the solution
# The function signature is expected to accept initial conditions, step size, range, and arrays for independent variable (x) and solution (y)
ode_solver.solve_ode(c_double(y0), c_double(y_prime0), c_double(dx), c_double(x_max), x_values_ctypes, y_values_ctypes, c_int(num_steps))

# Plot the results of the ODE solution
plt.figure(figsize=(10, 6))
plt.plot(np.arange(0, x_max + dx, dx), y_values, label="y(x)")
plt.title("Numerical Solution of a Differential Equation Using C Library")
plt.xlabel("x")
plt.ylabel("y(x)")
plt.grid()
plt.legend()
plt.show()

