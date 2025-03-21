import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load shared object file (use absolute path if needed)
dice_simulation = ctypes.CDLL('./ccode.so')

# Define function signatures
dice_simulation.sim_die_pmf.argtypes = [ctypes.POINTER(ctypes.c_double)]
dice_simulation.sim_die_pmf.restype = None
dice_simulation.theoretical_die_pmf.argtypes = [ctypes.POINTER(ctypes.c_double)]
dice_simulation.theoretical_die_pmf.restype = None
dice_simulation.compute_cdf.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
dice_simulation.compute_cdf.restype = None

# Number of sides on the die
sides = 6

# Define outcome values (1 to 6)
x_values = np.array([x + 1 for x in range(sides)])

# Arrays to store PMF and CDF values
pmf_sim = np.zeros(sides, dtype=np.double)
pmf_theory = np.zeros(sides, dtype=np.double)
cdf_sim = np.zeros(sides, dtype=np.double)
cdf_theory = np.zeros(sides, dtype=np.double)

# Call C functions for PMF
dice_simulation.sim_die_pmf(pmf_sim.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
dice_simulation.theoretical_die_pmf(pmf_theory.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))

# Compute CDF from PMF
dice_simulation.compute_cdf(pmf_sim.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                            cdf_sim.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
dice_simulation.compute_cdf(pmf_theory.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                            cdf_theory.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))

# Plot PMF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_values, pmf_theory, label="Theoretical PMF")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'green')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

markerline, stemlines, baseline = plt.stem(x_values, pmf_sim, label="Simulated PMF")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

plt.xlabel('Die Roll Outcome')
plt.ylabel('Probability')
plt.grid(True)
plt.legend(loc='best')
plt.show()

# Plot CDF
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_values, cdf_theory, label="Theoretical CDF")
plt.setp(markerline, 'markerfacecolor', 'blue')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

markerline, stemlines, baseline = plt.stem(x_values, cdf_sim, label="Simulated CDF")
plt.setp(markerline, 'markerfacecolor', 'purple')
plt.setp(stemlines, 'color', 'purple')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

plt.xlabel('Die Roll Outcome')
plt.ylabel('Cumulative Probability')
plt.grid(True)
plt.legend(loc='best')
plt.show()
