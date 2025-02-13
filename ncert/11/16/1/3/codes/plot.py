import numpy as np
import matplotlib.pyplot as plt
import scipy.stats
import ctypes

# Load shared object file
so_file = ctypes.CDLL("./bernoulli_simulation.so")

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

# Theoretical PMF (Binomial Distribution for n=4, p=0.5)
n = 4
p = 0.5
theoretical_pmf = [scipy.stats.binom.pmf(k, n, p) for k in range(n+1)]

# Plot PMF
plt.stem(values, empirical_pmf, linefmt='r-', markerfmt='ro', basefmt=" ", label="Simulated PMF")
plt.scatter(range(n+1), theoretical_pmf, color='b', label="Theoretical PMF", s=20, zorder=4)

plt.xlabel("x(Number of Heads)")
plt.ylabel("PMF")
plt.xticks(range(n+1))
plt.legend(loc='upper right', fontsize=10)
plt.grid(True, linestyle='--', alpha=0.6)
plt.show()


