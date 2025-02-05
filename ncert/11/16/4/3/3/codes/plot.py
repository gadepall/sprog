import numpy as np
import matplotlib.pyplot as plt

# Load data from the file
data = np.loadtxt("results.txt", skiprows=1)

x = data[:, 0]
pmf_sim = data[:, 1]
cdf_sim = data[:, 2]
pmf_theory = data[:, 3]
cdf_theory = data[:, 4]

# Plot PMF separately
plt.figure(figsize=(6, 4))
plt.stem(x, pmf_sim, linefmt='r', markerfmt='ro', basefmt=" ", label='Simulated PMF')
plt.stem(x, pmf_theory, linefmt='g', markerfmt='go', basefmt=" ", label='Theoretical PMF')
plt.xlabel("x")
plt.ylabel("p(x)")
plt.legend()
plt.grid()
plt.title("Probability Mass Function (PMF)")
plt.savefig("pmf_plot.png")
plt.show()

# Plot CDF separately
plt.figure(figsize=(6, 4))
plt.step(x, cdf_sim, where='mid', label='Simulated CDF', color='r', linestyle='--')
plt.step(x, cdf_theory, where='mid', label='Theoretical CDF', color='g')
plt.xlabel("x")
plt.ylabel("F(x)")
plt.legend()
plt.grid()
plt.title("Cumulative Distribution Function (CDF)")
plt.savefig("cdf_plot.png")
plt.show()

