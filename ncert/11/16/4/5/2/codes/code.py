import numpy as np
import matplotlib.pyplot as plt

# Define x values (possible outcomes)
x_values = np.array([0, 1])
pmf_values = np.array([0.6, 0.4])

# Plot PMF
plt.figure(figsize=(10,4))
plt.subplot(1,2,1)
plt.stem(x_values, pmf_values)
plt.xlabel("x")
plt.ylabel("P(X=x)")
plt.title("PMF of Bernoulli(0.4)")
plt.xticks([0, 1])
plt.ylim(0, 1)

# Define x values for CDF plot
x_cdf = np.array([-1, 0, 1, 2])
cdf_values = np.array([0, 0.6, 1, 1])

# Plot CDF
plt.subplot(1,2,2)
plt.step(x_cdf, cdf_values, where="post")
plt.xlabel("x")
plt.ylabel("F(X)")
plt.title("CDF of Bernoulli(0.4)")
plt.xticks([0, 1])
plt.yticks([0, 0.6, 1])
plt.ylim(-0.1, 1.1)

plt.savefig('../figs/fig.png')

