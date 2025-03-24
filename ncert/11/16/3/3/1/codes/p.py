import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
pmf_lib = ctypes.CDLL('./c.so')

# Define output array
SIDES = 6
pmf = (ctypes.c_double * SIDES)()

# Call the C function to calculate the PMF
pmf_lib.calculate_pmf(pmf)

# Convert the results to a NumPy array
pmf_values = np.array(pmf[:])

# Define the x-axis labels (die outcomes)
x = np.arange(1, SIDES + 1)

# Create the stem plot
plt.stem(x, pmf_values, basefmt=" ", use_line_collection=True)
plt.title("PMF of Rolling a Die")
plt.xlabel("Die Outcome")
plt.ylabel("Probability")
plt.xticks(x)  # Ensure all outcomes are shown on the x-axis
plt.grid()
plt.show()

