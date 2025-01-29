import ctypes
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./pmf_cdf_calculator.so')

# Define the function signatures
NUM_VALUES = 5
lib.get_pmf.argtypes = [ctypes.POINTER(ctypes.c_double)]
lib.get_cdf.argtypes = [ctypes.POINTER(ctypes.c_double)]
lib.get_values.argtypes = [ctypes.POINTER(ctypes.c_double)]

# Allocate arrays for PMF, CDF, and values
pmf = (ctypes.c_double * NUM_VALUES)()
cdf = (ctypes.c_double * NUM_VALUES)()
values = (ctypes.c_double * NUM_VALUES)()

# Call C functions to calculate PMF, CDF, and values
lib.get_pmf(pmf)
lib.get_cdf(cdf)
lib.get_values(values)

# Convert the C arrays to Python lists
pmf = list(pmf)
cdf = list(cdf)
values = list(values)

# Plot PMF
plt.figure(figsize=(10, 6))
plt.subplot(2, 1, 1)
plt.stem(values, pmf, basefmt=" ", use_line_collection=True, linefmt='blue', markerfmt='bo', label="PMF")
plt.title("Probability Mass Function (PMF)")
plt.xlabel("Net Money")
plt.ylabel("Probability")
plt.grid(alpha=0.5)
plt.legend()

# Plot CDF
plt.subplot(2, 1, 2)
plt.step(values, cdf, where='post', color='green', label="CDF")
plt.title("Cumulative Distribution Function (CDF)")
plt.xlabel("Net Money")
plt.ylabel("Cumulative Probability")
plt.grid(alpha=0.5)
plt.legend()

# Show plots
plt.tight_layout()
plt.show()

