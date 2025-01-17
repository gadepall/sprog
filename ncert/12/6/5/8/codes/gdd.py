import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the compiled C library
lib = ctypes.CDLL('./gradient.so')

# Define function prototypes
lib.gradient_descent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.gradient_descent.restype = ctypes.c_double

lib.gradient_ascent.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int, ctypes.c_double]
lib.gradient_ascent.restype = ctypes.c_double

# Define the Python function
def f(x):
    return np.sin(2 * x)

# Gradient descent/ascent parameters
learning_rate = 0.01  # decides the size of the jump
max_iter = 10000
tol = 1e-6
start_points = np.linspace(0, 2 * np.pi, 50)  # Multiple starting points

# Calling C code
# Initialize an empty list to store critical points (minima and maxima)
critical_points = []

# Iterate over a range of starting points (we are trying multiple initial guesses)
for x0 in start_points:
    # Call the 'gradient_descent' function to find the local minimum starting from the initial guess 'x0'
    # The function uses the learning rate, max iterations, and tolerance as parameters
    min_x = lib.gradient_descent(x0, learning_rate, max_iter, tol)

    # Call the 'gradient_ascent' function to find the local maximum starting from the initial guess 'x0'
    # This function also uses the learning rate, max iterations, and tolerance
    max_x = lib.gradient_ascent(x0, learning_rate, max_iter, tol)

    # Check if the found minimum ('min_x') is already in the list of critical points
    if not any(abs(min_x - cp) < 1e-3 for cp in critical_points):
        critical_points.append(min_x)

    # Similarly, check if the found maximum ('max_x') is already in the list of critical points
    if not any(abs(max_x - cp) < 1e-3 for cp in critical_points):
        critical_points.append(max_x)

# Once all the initial points have been processed, convert the list of critical points to a NumPy array
critical_points = np.unique(np.array(critical_points))

# Evaluate the function at the critical points
extrema = [(x, f(x)) for x in critical_points]

# Separate minima and maxima
minima = []
maxima = []
for x, y in extrema:
    if np.sign(f(x + 1e-3) - f(x)) < 0 and x < 2 * np.pi:  # Check slope for maxima
        maxima.append((x, y))
    elif np.sign(f(x+1e-3)-f(x))>0 and 0<x<2*np.pi:
        minima.append((x, y))

# Plot the function and both minima and maxima
x_vals = np.linspace(0, 2 * np.pi, 500)
y_vals = f(x_vals)

plt.figure(figsize=(10, 6))
plt.plot(x_vals, y_vals, label='f(x) = sin(2x)', color='blue')

# Plot maxima points
for x, y in maxima:
    if 0 <= x <= 2 * np.pi:  # Plot only maxima within the range [0, 2pi]
        plt.scatter(x, y, color='red', zorder=5, label=f"Max: ({x:.2f}, {y:.2f})")

# Plot minima points
for x, y in minima:
    if 0 <= x <= 2 * np.pi:  # Plot only minima within the range [0, 2pi]
        plt.scatter(x, y, color='green', zorder=5, label=f"Min: ({x:.2f}, {y:.2f})")

# Finalize the plot
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend(loc='best')
plt.grid(True)
plt.show()

# Display maxima and minima
print("Maxima:", maxima)
print("Minima:", minima)

