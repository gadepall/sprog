import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gen = ctypes.CDLL('./grad.so')

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
init_guess = 5.0  # Initial guess
step_size = 0.01  # Reduced learning rate
min_value = ctypes.c_double(0.0)
num_points = 100
tolerance = 1e-6  # Stricter tolerance

# Create a numpy array for the points
points = np.zeros((num_points, 2), dtype=np.double)

# Define the corrected derivative function
def f_derivative(x):
    return 2 * (x - 1) + x * (((x*x)/4) - 2)  # Derivative of L(a) = (a - 120)^2

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
y1=((min_value.value)*(min_value.value))/4
# Print the result
print(f"Converged value of 'a': {min_value.value}")
print(f"Corresponding y value: {y1}") 

# Plot the loss function and minimum point
fig, ax1 = plt.subplots()
# Generate values for plotting the loss function
a_values = np.linspace(-5, 5, num_points)
loss_values = (a_values - 1) ** 2 + ((a_values**2)/4 - 2) ** 2
ax1.plot(a_values, loss_values, c='r', label='L(a)')
ax1.scatter(min_value.value, (min_value.value - 1) ** 2 + ((min_value.value**2)/4 - 2) ** 2, c='b', label='Minimum Point')
ax1.set_xlim(-5, 5)
ax1.set_xlabel('a')
ax1.set_ylabel('L(a)')
plt.grid()
ax1.legend(loc="best")
plt.show()

