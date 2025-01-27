import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom, norm

n = 10 
p = 0.5
mu = n * p
sigma = np.sqrt(n * p * (1 - p))
x = np.arange(0, n+1)
pmf = binom.pmf(x, n, p)
plt.figure(figsize=(10, 6))
plt.stem(x, pmf, linefmt="r-")
x = np.linspace(0, n, 100)
y = norm.pdf(x, mu, sigma)
#plt.plot(x, y, 'r-')

n = 2 
p = 0.5
mu = n * p
x = np.arange(0, n+1)
sigma = np.sqrt(n * p * (1 - p))
pmf = binom.pmf(x, n, p)
plt.stem(x, pmf, linefmt="b-")
x = np.linspace(0, n, 100)
correction_factor = 1 / (1 + 1 / (4 * n))
y = norm.pdf(x, mu, sigma * correction_factor)
#plt.plot(x, y, 'b-')
plt.xlabel('Number of successes')
plt.ylabel('Probability')
plt.xticks(x)
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig("../figs/binomial.png")
plt.show()

