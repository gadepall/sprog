import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./lib.so')

# Fix the function signature
lib.generate_number.restype = ctypes.c_int
lib.generate_pmf.argtypes = [ctypes.POINTER(ctypes.c_double)]


x = np.array([1, 2, 3])
pmf = np.zeros(3, dtype=np.double)  # Ensure correct data type
pmf_theory=([1/3,0.5,1/6])
cmf_theory=np.cumsum(pmf_theory)
#cmf=np.cumsum(pmf)
cmf=([1/3,5/6,1])
# Convert numpy array to a C-compatible pointer
pmf_ctypes = pmf.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the function correctly
lib.generate_pmf(pmf_ctypes)

plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x,pmf_theory, label="theory")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
markerline, stemlines, baseline = plt.stem(x, pmf, label="sim")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.xlabel('x')
plt.ylabel('$p_X(k)$')
plt.legend(loc='best')
plt.grid(True)
plt.show()


plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x, cmf_theory, label="theory")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
markerline, stemlines, baseline = plt.stem(x, cmf, label="sim")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.xlabel('x')
plt.ylabel('$F_{X}(k)$')
plt.legend(loc='best')
plt.grid(True)
plt.show()

