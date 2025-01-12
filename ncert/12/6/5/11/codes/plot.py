import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gen = ctypes.CDLL('./solver.so')

# Define argument and return types for the functions
gen.generate_points.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
gen.generate_points.restype = None

FUNC_TYPE = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
gen.run_gradient_descent.argtypes = [
    ctypes.c_double, ctypes.c_double, FUNC_TYPE, 
    ctypes.c_double, ctypes.POINTER(ctypes.c_double)
]
gen.run_gradient_descent.restype = None

# Parameters
init_guess = 50.0  # Initial guess for 'a'
step_size = 0.1  # Learning rate
min_value = ctypes.c_double(0.0)
num_points = 100
tolerance = 1e-5

# Create a numpy array for the points
points = np.zeros((num_points, 2), dtype=np.double)

# Define the derivative function
def f_derivative(a):
    return 2 * (a - 120)

# Call the generate_points function
gen.generate_points(
    points.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
    num_points
)

# Reshape the points array for easier plotting
points = points.reshape((num_points, 2))

# Call the run_gradient_descent function
gen.run_gradient_descent(
    init_guess, step_size, FUNC_TYPE(f_derivative), tolerance, ctypes.byref(min_value)
)

# Print the result
print(f"Converged value of 'a': {min_value.value}")

# Plot the loss function and minimum point
fig, ax1 = plt.subplots()
# Generate values for plotting the loss function
a_values = np.linspace(0, 200, num_points)
loss_values = (a_values - 120) ** 2
ax1.plot(a_values, loss_values, c='r', label='L(a)')
ax1.scatter(min_value.value, (min_value.value - 120) ** 2, c='b', label='Minimum Point')
ax1.set_xlim(0, 200)
ax1.set_xlabel('a')
ax1.set_ylabel('L(a)')
ax1.legend(loc="best")
plt.savefig('../figs/fig1.png')
plt.show()

# Plot the function f(x) with a = 120
fig, ax2 = plt.subplots()
x_values = np.linspace(0, 2, 100)  # Interval [0, 2]
a = 120  # Value of 'a'
f_values = x_values**4 - 62 * x_values**2 + a * x_values + 9
ax2.plot(x_values, f_values, c='g', label=r'$f(x) = x^4 - 62x^2 + 120x + 9$')
ax2.axvline(x=1, color='b', linestyle='--', label='x = 1 (Maximum Point)')
ax2.set_xlabel('x')
ax2.set_ylabel('f(x)')
ax2.legend(loc="best")
plt.savefig('../figs/fig2.png')
plt.show()

