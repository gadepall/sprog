import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
lottery = ctypes.CDLL('./lottery.so')

# Define the function prototype for the simulate_lottery function
lottery.simulate_lottery.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double)]

# Parameters
simulations = 10000000  # Total number of simulations
interval = 10000        # Interval for tracking relative frequency
num_intervals = simulations // interval

# Allocate space for results
relative_frequencies = (ctypes.c_double * num_intervals)()

# Call the C function
lottery.simulate_lottery(simulations, interval, relative_frequencies)

# Convert results to NumPy array
relative_frequencies = np.array(relative_frequencies)
print(relative_frequencies)
# Compute the true probability
total_combinations = np.math.comb(20, 6)
true_probability = 1 / total_combinations

# PMF
pmf = [1 - true_probability, true_probability]
outcomes = [0, 1]  # 0 = Lose, 1 = Win

# CDF
cdf = np.cumsum(pmf)

# Plot PMF
plt.figure(figsize=(10, 6))
plt.bar(outcomes, pmf, color='skyblue', alpha=0.7, label='PMF')
plt.xticks(outcomes, ['Lose', 'Win'])
plt.ylabel('Probability')
plt.title('PMF of Winning the Lottery')
plt.legend()
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('../figs/pmf.png')
plt.show()

# Plot CDF
plt.figure(figsize=(10, 6))
plt.step(outcomes, cdf, where='post', color='green', label='CDF')
plt.xticks(outcomes, ['Lose', 'Win'])
plt.ylabel('Cumulative Probability')
plt.title('CDF of Winning the Lottery')
plt.legend()
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('../figs/cdf.png')
plt.show()

# Plot convergence of relative frequency with zoomed-in y-axis in terms of 1e-5
plt.figure(figsize=(10, 6))
x_values = range(interval, simulations + 1, interval)
plt.plot(x_values, relative_frequencies, label='Relative Frequency', color='blue')
plt.axhline(true_probability, color='red', linestyle='--', label='True Probability')

# Zoom in on the y-axis with terms of 1e-5
# Scale the y-axis values to be in the range of 1e-5
y_min = np.floor(min(min(relative_frequencies), true_probability) * 1e5) / 1e5
y_max = np.ceil(max(max(relative_frequencies), true_probability) * 1e5) / 1e5

plt.ylim(y_min, y_max)

# Set ticks for y-axis to be in terms of 1e-5 intervals
plt.yticks(np.arange(y_min, y_max + 0.00001, 0.00001))

plt.xlabel('Number of Trials')
plt.ylabel('Probability')
plt.title('Convergence of Relative Frequency to True Probability')
plt.legend()
plt.grid(alpha=0.7)
plt.savefig('../figs/fig.png')
plt.show()

