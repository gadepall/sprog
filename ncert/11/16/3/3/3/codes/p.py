import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
die_roll_lib = ctypes.CDLL('./kedar.so')

# Define the function signatures
die_roll_pmf = die_roll_lib.die_roll_pmf
die_roll_pmf.restype = ctypes.POINTER(ctypes.c_double)
die_roll_pmf.argtypes = [ctypes.c_int]

free_pmf = die_roll_lib.free_pmf
free_pmf.argtypes = [ctypes.POINTER(ctypes.c_double)]

def call_die_roll_pmf(trials):
    result_ptr = die_roll_pmf(ctypes.c_int(trials))
    if not result_ptr:
        raise ValueError("Error in die_roll_pmf: check input values.")
    pmf = [result_ptr[i] for i in range(6)]
    free_pmf(result_ptr)
    return pmf

def theoretical_die_roll_pmf():
    return [1/6 for _ in range(6)]

# Parameters
trials = 10000

# Simulated PMF
pmf = call_die_roll_pmf(trials)

# Theoretical PMF
theory_pmf = theoretical_die_roll_pmf()

# Compute CDFs
cdf = np.cumsum(pmf)
theory_cdf = np.cumsum(theory_pmf)

# Plot the PMF
x = np.arange(1, 7)  # Outcomes on the die
plt.figure(figsize=(8, 6))
plt.stem(x, pmf, linefmt='r-', markerfmt='ro', basefmt=" ", label="Simulated PMF")
plt.stem(x, theory_pmf, linefmt='g-', markerfmt='go', basefmt=" ", label="Theoretical PMF")
plt.xlabel('Die Roll Outcome')
plt.ylabel('Probability')
plt.title('Probability Mass Function (PMF) of Die Roll')
plt.legend()
plt.grid()
plt.show()

# Plot the CDF
plt.figure(figsize=(8, 6))
plt.step(x, cdf, where='post', label="Simulated CDF", color='red')
plt.step(x, theory_cdf, where='post', label="Theoretical CDF", color='green')
plt.xlabel('Die Roll Outcome')
plt.ylabel('Cumulative Probability')
plt.title('Cumulative Distribution Function (CDF) of Die Roll')
plt.legend()
plt.grid()
plt.show()

