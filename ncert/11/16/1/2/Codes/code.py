import numpy as np
import matplotlib.pyplot as plt
import scipy.stats
import ctypes
import os

# Load compiled shared object file
so_file = ctypes.CDLL(os.path.abspath("./code.so"))

# Define function prototype
so_file.run_simulation.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]

# Number of trials
NUM_TRIALS = 100000

# Allocate memory for results
results = (ctypes.c_int * NUM_TRIALS)()

# Run simulation
so_file.run_simulation(results, NUM_TRIALS)

# Convert results to numpy array
results_array = np.array(results)

# Compute empirical PMF
values, counts = np.unique(results_array, return_counts=True)
empirical_pmf = counts / NUM_TRIALS

# Theoretical PMF (sum of two independent dice)
probabilities = {
    2: 1/36,  3: 2/36,  4: 3/36,  5: 4/36,  6: 5/36,  
    7: 6/36,  8: 5/36,  9: 4/36, 10: 3/36, 11: 2/36, 12: 1/36
}
theoretical_pmf = [probabilities[k] for k in range(2, 13)]

# Plot PMF
plt.stem(values, empirical_pmf, linefmt='r-', markerfmt='ro', basefmt=" ", label="Simulated PMF")
plt.scatter(range(2, 13), theoretical_pmf, color='b', label="Theoretical PMF", s=20, zorder=3)

plt.xlabel("Sum of Two Dice")
plt.ylabel("PMF")
plt.xticks(range(2, 13))
plt.legend(loc='upper right', fontsize=10)
plt.grid(True, linestyle='--', alpha=0.6)
plt.show()

