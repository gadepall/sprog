#Code by S.Sai Akshita,on 4th feb 2025
#importing necessary libraries
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom

# Parameters for binomial random variable
n = 2   # Number of trials
p = 0.4 # Probability of success

# values of X
x = np.arange(0, n+1)

# Compute PMF values
pmf_values = binom.pmf(x, n, p)

# Print PMF values
for i in range(len(x)):
    print(f"P(X = {x[i]}) = {pmf_values[i]:.4f}")

# Plot the PMF
plt.stem(x,pmf_values)
#for j in range(0,n+1):
 #   plt.plot(x[j],pmf_values[j],color='red')

#plt.plot(x, pmf_values, color='red')
x=np.array(x)
pmf_values=np.array(pmf_values)
plt.xlabel("X (Number of students in Section A)")
plt.ylabel("P(X)")
#plt.title("PMF of X ~ Binomial(2, 0.4)")
plt.xticks(x)
plt.ylim(0, max(pmf_values) + 0.05)
plt.savefig('probability.png')

# Show probability values on the bars
for i in range(len(x)):
    plt.text(x[i], pmf_values[i] + 0.01, f"{pmf_values[i]:.2f}", ha='center')

plt.show()

