import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
# Replace 'solver.so' with the actual compiled .so file name
binomial_lib = ctypes.CDLL('./ct.so')

# Define the function signature
binomial_rv = binomial_lib.binomial_rv
binomial_rv.restype = ctypes.POINTER(ctypes.c_double)  # Function returns a pointer to double
binomial_rv.argtypes = [ctypes.c_double, ctypes.c_int]  # Arguments: double pval, int no_events

def call_binomial_rv(pval, no_events):
    # Call the C function
    result_ptr = binomial_rv(ctypes.c_double(pval), ctypes.c_int(no_events))
    
    # Convert the result into a Python list
    pmf = [result_ptr[i] for i in range(no_events + 1)]
    
    # Free the memory allocated in C (since malloc was used)
    binomial_lib.free(result_ptr)
    
    return pmf

def factorial(x):
    if x == 0 or x == 1:
        return 1
    else:
        ftrl = 1
        for i in range(1, x + 1):
            ftrl *= i
        return ftrl

def comb(n, k):
    if k <= n:
        return factorial(n) / (factorial(k) * factorial(n - k))
    else:
        return 0

def theoretical_binomial(pval, no_events, events_with_fav_outcomes):
    return comb(no_events, events_with_fav_outcomes) * ((1 - pval) ** (no_events - events_with_fav_outcomes)) * (pval ** events_with_fav_outcomes)

# Parameters
pval = 0.5  # Probability of success
no_events = 3  # Number of trials

# Get the PMF from the C function
pmf = call_binomial_rv(pval, no_events)

# Compute the theoretical PMF
theory_pmf = [theoretical_binomial(pval, no_events, k) for k in range(no_events + 1)]

# Prepare data for plotting
x = np.arange(len(pmf))
y = np.array(pmf)
theory_y = np.array(theory_pmf)
cdf = np.cumsum(pmf)
theory_cdf = np.cumsum(theory_pmf)

# Modify x and cdf to start from -1
x_with_neg1 = np.insert(x, 0, -1)
cdf_with_neg1 = np.insert(cdf, 0, 0)
theory_cdf_with_neg1 = np.insert(theory_cdf, 0, 0)

# Plot the PMF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x, theory_y, label="theory")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
markerline, stemlines, baseline = plt.stem(x, y, label="sim")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.xlabel('x')
plt.ylabel('$p_X(k)$')
plt.legend(loc='best')
plt.grid(True)
plt.title("Probability Mass Function (PMF)")
plt.show()

# Plot the CDF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_with_neg1, theory_cdf_with_neg1, label="theory")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
markerline, stemlines, baseline = plt.stem(x_with_neg1, cdf_with_neg1, label="sim")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.xlabel('x')
plt.ylabel('$F_{X}(k)$')
plt.legend(loc='best')
plt.grid(True)
plt.title("Cumulative Distribution Function (CDF)")
plt.show()
