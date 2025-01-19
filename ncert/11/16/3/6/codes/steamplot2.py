import numpy as np
import matplotlib.pyplot as plt

# Define the outcomes and probabilities
outcomes = ["Man", "Woman"]
pmf = np.array([0.4, 0.6])  # Probabilities of selecting a man or woman
cdf = np.cumsum(pmf)  # Cumulative probabilities

# Plotting the CDF as a stem plot
plt.stem(outcomes, cdf, basefmt=" ")
plt.xlabel("Council Member Type")
plt.ylabel("Cumulative Probability (CDF)")
plt.title("CDF of Selecting a Council Member")
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.show()

