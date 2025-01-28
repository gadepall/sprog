import numpy as np
import matplotlib.pyplot as plt
import math
from ctypes import CDLL, c_double

# Load the shared object (.so) file
lib = CDLL('./code.so')

# Function to compute the update rule using the shared library
def update_hc(hc, h, alpha, tan_alpha_squared):
    # Call the C function from the shared library
    lib.update_hc.restype = c_double
    return lib.update_hc(c_double(hc), c_double(h), c_double(alpha), c_double(tan_alpha_squared))

# Parameters
h = 10.0  # Example value for h
alpha = 0.01  # Learning rate
tan_alpha = math.tan(math.pi / 6)  # Example angle in radians
tan_alpha_squared = tan_alpha ** 2
hc_initial = 1.0  # Initial guess for hc
iterations = 100  # Number of iterations

# Arrays to store values for plotting
hc_values = [hc_initial]
iteration_indices = list(range(iterations))

# Iterative process
hc = hc_initial
for _ in iteration_indices[1:]:
    hc = update_hc(hc, h, alpha, tan_alpha_squared)
    hc_values.append(hc)

# Plotting
plt.plot(iteration_indices, hc_values, label="hc values", color="blue")
plt.xlabel("Iterations")
plt.ylabel("hc")
plt.title("Iterative Update of hc")
plt.legend()
plt.grid(True)
plt.savefig("../figure/fig.png")
plt.show()

