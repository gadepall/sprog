import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the compiled C shared library
lib = ctypes.CDLL('./grad.so')

# Define the function signatures for gradient descent and ascent
lib.gradientdesc.argtypes = [
    ctypes.c_int,                            # int n
    ctypes.POINTER(ctypes.c_double),         # double x[n]
    ctypes.c_double,                         # double alpha
    ctypes.c_double,                         # double x_0
    ctypes.c_double                          # double threshold
]

lib.gradientasc.argtypes = [
    ctypes.c_int,                            # int n
    ctypes.POINTER(ctypes.c_double),         # double x[n]
    ctypes.c_double,                         # double alpha
    ctypes.c_double,                         # double x_0
    ctypes.c_double                          # double threshold
]

# Parameters for gradient descent and ascent
alpha = 0.01       # Learning rate
x_0 = 0.0      # Initial guess
threshold = 1e-5   # Convergence threshold
n = 10000          # Max iterations

# Create numpy arrays to hold the results from C
x_desc_values = np.zeros(n, dtype=np.float64)
x_asc_values = np.zeros(n, dtype=np.float64)

# Convert numpy arrays to C-compatible pointers
x_desc_ptr = x_desc_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
x_asc_ptr = x_asc_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the C gradient descent function
lib.gradientdesc(n, x_desc_ptr, alpha, x_0, threshold)

# Call the C gradient ascent function
lib.gradientasc(n, x_asc_ptr, alpha, x_0, threshold)

# Truncate `x_desc_values` and `x_asc_values` to only include computed iterations
x_desc_values = x_desc_values[x_desc_values != 0]
x_asc_values = x_asc_values[x_asc_values != 0]

# Compute the function values for the descent and ascent paths
y_desc_values = (1 - x_desc_values + x_desc_values**2) / (1 + x_desc_values + x_desc_values**2)
y_asc_values = (1 - x_asc_values + x_asc_values**2) / (1 + x_asc_values + x_asc_values**2)

# Generate the theoretical function curve
x = np.linspace(-2, 2, 400)
y = (1 - x + x**2) / (1 + x + x**2)

# Define the maximum point for annotation
x_max = 1
y_max = (1 - x_max + x_max**2) / (1 + x_max + x_max**2)
x_min= -1
y_min = (1 - x_min + x_min**2) / (1 + x_min + x_min**2)
# Plot the results
plt.figure(figsize=(12, 8))
plt.plot(x, y, color='blue', label="theory")  # Theoretical curve
plt.plot(x_desc_values, y_desc_values, color='red', marker='o', linestyle='--', label="sim1")  # Gradient descent
plt.plot(x_asc_values, y_asc_values, color='green', marker='o', linestyle='--', label="sim2")  # Gradient ascent

# Mark and annotate the maximum point
plt.scatter(x_max, y_max, color="orange")  # Highlight the point
plt.annotate(
    f"({x_max:.2f}, {y_max:.2f})",  # Text to display
    (x_max, y_max),  # Point to annotate
    textcoords="offset points",  # Offset the text
    xytext=(10, 10),  # Offset the text by 10 points
)
plt.scatter(x_min, y_min, color="orange")  # Highlight the point
plt.annotate(
    f"({x_min:.2f}, {y_min:.2f})",  # Text to display
    (x_min, y_min),  # Point to annotate
    textcoords="offset points",  # Offset the text
    xytext=(10, 10),  # Offset the text by 10 points
    
)


# Add labels, legend, and grid
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid(True)
plt.show()

