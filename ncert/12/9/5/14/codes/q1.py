import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL("./q1.so")

# Define the generate_points function's signature
lib.generate_points.argtypes = [
    ctypes.c_double,  # x_start
    ctypes.c_double,  # x_end
    ctypes.c_double,  # y_initial
    ctypes.POINTER(ctypes.c_double),  # x array
    ctypes.POINTER(ctypes.c_double),  # y array
]

# Define constants
POINTS = 100
x_start = 0.2
x_end = 1.0
y_initial = 0.4361

# Prepare arrays to store the results
x = (ctypes.c_double * POINTS)()
y = (ctypes.c_double * POINTS)()

# Call the function
lib.generate_points(x_start, x_end, y_initial, x, y)

# Convert C arrays to NumPy arrays
x_values = np.array(x)
y_values = np.array(y)

# Define the mathematical function, adjusting to start from y_initial and x_initial
def y_function_adjusted(x, x_initial, y_initial):
    # Ensure the argument for arccos stays in the range [-1, 1]
    argument = 1 + np.log(x)
    argument = np.clip(argument, -1, 1)  # Clip the values within the valid range for arccos
    return x * np.arccos(argument)

# Generate x values for the mathematical function
x_math = np.linspace(x_start, x_end, 500)  # Ensure x values match the range of shared library
y_math = y_function_adjusted(x_math, x_start, y_initial)

# Plot both graphs
plt.figure(figsize=(10, 6))

# Plot the data from the shared library function
plt.plot(
    x_values,
    y_values,
    marker="o",
    linestyle="-",
    color="blue",
    label="sim",
)

# Plot the adjusted mathematical function
plt.plot(
    x_math,
    y_math,
    linestyle="--",
    color="red",
    label=r"Theory",
)

# Customize the plot
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.grid(True)
plt.legend()
plt.show()

