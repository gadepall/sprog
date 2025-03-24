import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom

# Parameters
N = 100000  # Number of trials
n_flips = 3  # Number of coin flips per trial

# Simulate coin tosses (0 = Head, 1 = Tail)
trials = np.random.randint(0, 2, size=(N, n_flips))

# Compute sum of tails in each trial
sum_tails = np.sum(trials, axis=1)

# Compute empirical PMF
values, counts = np.unique(sum_tails, return_counts=True)
empirical_pmf = counts / N

# Theoretical PMF using Binomial distribution
th_values = np.arange(0, n_flips + 1)
theoretical_pmf = binom.pmf(th_values, n_flips, 0.5)

# Plot comparison
plt.figure(figsize=(6, 4))
plt.stem(values, empirical_pmf, linefmt='b--', markerfmt='bo', basefmt='k-', label='Simulated PMF')
plt.stem(th_values, theoretical_pmf, linefmt='r-', markerfmt='ro', basefmt='k-', label='Theoretical PMF')

# Labels and title
plt.xlabel('Number of Tails')
plt.ylabel('Probability')
plt.title('Comparison of Theoretical and Simulated PMF')
plt.xticks(th_values)  # Ensure x-axis has correct values
plt.legend()
plt.grid()
plt.show()

