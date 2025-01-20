import matplotlib.pyplot as plt
import numpy as np
import ctypes

func = ctypes.CDLL('./func.so')

func.calculate_y.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.POINTER(ctypes.c_double)]
func.calculate_y.restype = None

# Define a Python function that uses the C function
def calculate_y(n, h):
    # Create an array to hold the results
    y = np.zeros(n, dtype=np.double)  # Allocate space for `n` double values
    # Call the C function
    func.calculate_y(n, h, y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
    return y.tolist()  # Convert the result to a Python list

# Parameters
n_c = 1000  # Number of points
h = 0.01  # Step size


# Calculate the values of y using the C function
y_values = calculate_y(n_c, h)

# Time array
x_itr = np.linspace(0, n_c * h, n_c)

# Initialize y_theory array
y_theory = [0] * n_c  # Theoretical output (initialize to zero)
# Define delta function for theoretical computation
def delta(n, k):
    return 1 if n == k else 0

# Compute theoretical values
for n in range(n_c - 3):
     y_theory[n + 3] = -3*y_theory[n + 1] + y_theory[n] + 3*y_theory[n+2]+ (h/2.0)**3 * delta(n, 0)-(h*h)*delta(n,0)


for n in range(n_c - 3):
    y_theory[n + 3] *= -1	
y_theory = np.array(y_theory)

# Plot the results
plt.plot(x_itr, y_theory, label="Theoretical plot", color="red")
plt.plot(x_itr, y_values, label="Simulated plot", color="green", linestyle = '-.')
plt.xlabel("Time (s)")
plt.ylabel("Output y(n)")
plt.legend()
plt.grid(True)
plt.savefig('../figs/fig.png')

