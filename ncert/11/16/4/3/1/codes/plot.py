import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./lib.so')

# Fix the function signature
lib.generate_pmf.argtypes = [ctypes.POINTER(ctypes.c_double)]

x = np.array([1, 2, 3])
pmf = np.zeros(3, dtype=np.double)  # Initialize with correct data type
pmf_theory = np.array([2/6, 3/6, 1/6])
cmf_theory = np.cumsum(pmf_theory)

# Convert numpy array to a C-compatible pointer
pmf_ctypes = pmf.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the function to generate PMF through simulation
lib.generate_pmf(pmf_ctypes)

# Plot PMF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x, pmf_theory, label="Theoretical PMF")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
markerline, stemlines, baseline = plt.stem(x, pmf, label="Simulated PMF")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.xlabel('x')
plt.ylabel('$p_X(k)$')
plt.legend(loc='best')
plt.grid(True)
plt.show()

# Plot CMF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x, cmf_theory, label="Theoretical CDF")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
markerline, stemlines, baseline = plt.stem(x, np.cumsum(pmf), label="Simulated CDF")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.xlabel('x')
plt.ylabel('$F_{X}(k)$')
plt.legend(loc='best')
plt.grid(True)
plt.show()

# Output P(2) from simulation
print(f"Theoretical P(2): {pmf_theory[1]}")
print(f"Simulated P(2): {pmf[1]}")

