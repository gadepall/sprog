import ctypes
import matplotlib.pyplot as plt
import numpy as np
import os

# Ensure the directory exists
os.makedirs('figs', exist_ok=True)

# Load C shared library
lib = ctypes.CDLL('./6.so')
lib.main()

# Simulate die throws
N = 1000
die_rolls = np.random.randint(1, 7, N)
counts, bins = np.histogram(die_rolls, bins=np.arange(1, 8), density=True)
cdf = np.cumsum(counts)

# Plot PMF and CDF
plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.bar(bins[:-1], counts, width=0.2, align='center', alpha=0.7, color='b', edgecolor='black')
plt.xlabel('Die Roll Outcome')
plt.ylabel('Probability')
plt.title('PMF of Die Roll')

plt.subplot(1, 2, 2)
plt.bar(bins[:-1], cdf, width=0.2, align='center', alpha=0.7, color='r', edgecolor='black')
plt.xlabel('Die Roll Outcome')
plt.ylabel('Cumulative Probability')
plt.title('CDF of Die Roll')

plt.tight_layout()

# Save the figure
plt.savefig('figs/fig.png', bbox_inches='tight')
plt.show()
