import numpy as np
import matplotlib.pyplot as plt

# Define theoretical PMF and CDF for the combined outcomes
def theoretical_pmf(index):
    return 1 / 18  # Each outcome has a probability of 1/18

def theoretical_cdf(index):
    return (index + 1) / 18  # CDF increases linearly

# X values for discrete PMF and CDF
x_values = np.arange(18)  # 18 possible outcomes
theoretical_pmf_values = np.array([theoretical_pmf(x) for x in x_values])
theoretical_cdf_values = np.array([theoretical_cdf(x) for x in x_values])

# Load the shared library
import ctypes
lib = ctypes.CDLL('./code.so')

# Define the function prototype
lib.monte_carlo.argtypes = [
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double)
]
lib.monte_carlo.restype = None

# Number of trials
num_trials = 1000000

# Allocate arrays for pmf and cdf
pmf = (ctypes.c_double * 18)()
cdf = (ctypes.c_double * 18)()

# Call the simulation
lib.monte_carlo(num_trials, pmf, cdf)

# Convert to numpy arrays
pmf_array = np.array(pmf)
cdf_array = np.array(cdf)

# PMF Plot with plt.stem
plt.figure(figsize=(8, 6))
plt.stem(x_values, pmf_array, linefmt='b-', markerfmt='bo', basefmt="gray", label='Simulated PMF')
plt.stem(x_values, theoretical_pmf_values, linefmt='r-', markerfmt='ro', basefmt="gray", label='Theoretical PMF')
plt.xlabel('Outcome Index')
plt.ylabel('PMF: $P(X=x)$')
plt.title('Probability Mass Function (PMF) of Combined Outcomes')
plt.grid(True)
plt.legend()
plt.savefig("../figs/pmf_combined_stem.png")  # Save the PMF plot
plt.show()

# CDF Plot with plt.stem
plt.figure(figsize=(8, 6))
plt.stem(x_values, cdf_array, linefmt='b-', markerfmt='bo', basefmt="gray", label='Simulated CDF')
plt.stem(x_values, theoretical_cdf_values, linefmt='r-', markerfmt='ro', basefmt="gray", label='Theoretical CDF')
plt.xlabel('Outcome Index')
plt.ylabel('CDF: $F_X(x)$')
plt.title('Cumulative Distribution Function (CDF) of Combined Outcomes')
plt.grid(True)
plt.legend()
plt.savefig("../figs/cdf_combined_stem.png")  # Save the CDF plot
plt.show()
