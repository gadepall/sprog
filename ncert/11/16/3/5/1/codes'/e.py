import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load shared object files
temp = ctypes.CDLL('./q.so')  # Ensure the correct shared library name

# Define function signatures for func.so
temp.probability_for_sum.argtypes = [ctypes.c_int]
temp.probability_for_sum.restype = ctypes.c_double

# The sums we are interested in range from 2 to 12 (both inclusive)
x = np.array([i for i in range(2, 13)])

# Simulating PMF values for each sum (2 to 12)
pmfsim_y = np.array([temp.probability_for_sum(i) for i in x])

# Plot PMF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x, pmfsim_y, label="Simulation", basefmt=" ", use_line_collection=True)
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.xlabel('Sum')
plt.ylabel('Probability Mass Function (PMF)')
plt.title('PMF of the Sum of Coin and Die')
plt.legend(loc='best')
plt.grid(True)
plt.savefig("../figs/pmf.png")
plt.show()

# Calculate the CDF by summing the PMF values (cumulative sum)
cdfsim_y = np.cumsum(pmfsim_y)

# Plot CDF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x, cdfsim_y, label="Simulation", basefmt=" ", use_line_collection=True)
plt.setp(markerline, 'markerfacecolor', 'blue')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.xlabel('Sum')
plt.ylabel('Cumulative Distribution Function (CDF)')
plt.title('CDF of the Sum of Coin and Die')
plt.legend(loc='best')
plt.grid(True)
plt.savefig("../figs/cdf.png")
plt.show()
