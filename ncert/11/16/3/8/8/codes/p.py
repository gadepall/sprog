import ctypes
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./c1.so')

# Create an array to hold PMF values (4 elements for 0 to 3 tails)
pmf = (ctypes.c_double * 4)()

# Call the C function to calculate PMF
lib.get_probabilities(pmf)

# Convert the PMF to a Python list
pmf_values = [pmf[i] for i in range(4)]

# Plotting the PMF
x = [0, 1, 2, 3]  # Number of tails
plt.stem(x, pmf_values, use_line_collection=True)
plt.xlabel('Number of Tails')
plt.ylabel('Probability')
plt.show()

