import numpy as np
import matplotlib.pyplot as plt

# Define PMF for a fair die
def pmf(x):
    if x in [1, 2, 3, 4, 5, 6]:
        return 1/6
    else:
        return 0

# Define CDF for a fair die
def cdf(x):
    if x < 1:
        return 0
    elif x < 2:
        return 1/6
    elif x < 3:
        return 2/6
    elif x < 4:
        return 3/6
    elif x < 5:
        return 4/6
    elif x < 6:
        return 5/6
    else:
        return 1

# X values for discrete PMF and CDF
x_values = np.arange(0, 9)  # Only valid die faces (1 to 6)
pmf_values = np.array([pmf(x) for x in x_values])
cdf_values = np.array([cdf(x) for x in x_values])

# PMF Plot
plt.figure(figsize=(8, 6))
plt.stem(x_values, pmf_values, linefmt='r-', markerfmt='ro', basefmt="gray")
plt.xlabel('x')
plt.ylabel('PMF: $P(X=x)$')
plt.title('Probability Mass Function (PMF) of a Die Roll')
plt.grid(True)
plt.savefig("../figs/pmf_die.png")
plt.show()

# CDF Plot
plt.figure(figsize=(8, 6))
plt.stem(x_values, cdf_values, linefmt='b-', markerfmt='bo', basefmt="gray")
plt.xlabel('x')
plt.ylabel('CDF: $F_X(x)$')
plt.title('Cumulative Distribution Function (CDF) of a Die Roll')
plt.grid(True)
plt.savefig("../figs/cdf_die.png")
plt.show()

