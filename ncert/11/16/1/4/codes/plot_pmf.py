import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
coin_die_lib = ctypes.CDLL('./coin_die_sim.so')

# Define the argument and return types for the simulate function
coin_die_lib.simulate.argtypes = [ctypes.c_int, np.ctypeslib.ndpointer(dtype=np.int32, flags="C_CONTIGUOUS")]
coin_die_lib.simulate.restype = None

# Simulation parameters
num_trials = 100000
counts = np.zeros(7, dtype=np.int32) # Array to store counts for sums 1 to 7

# Seed the random number generator
np.random.seed(42)  # for reproducibility
seed = np.random.randint(0, 2**32 - 1) # Generate a random seed
ctypes.cdll.LoadLibrary('libc.so.6').srand(seed) # Seed the C RNG

# Run the simulation
coin_die_lib.simulate(num_trials, counts)

# Calculate the PMF
pmf = counts / num_trials

# Theoretical PMF
theoretical_pmf = np.array([1/12, 1/6, 1/6, 1/6, 1/6, 1/6, 1/12])

# Plotting - STEM PLOT
x = np.arange(1, 8)

# Simulated PMF Stem Plot
markerline, stemlines, baseline = plt.stem(x, pmf, linefmt='C0-', markerfmt='C0o', basefmt=" ", label='Simulated PMF')
plt.setp(stemlines, linewidth=0.5) # Set linewidth for stemlines

# Theoretical PMF Stem Plot
markerline, stemlines, baseline = plt.stem(x, theoretical_pmf, linefmt='C1-', markerfmt='C1o', basefmt=" ", label='Theoretical PMF')
plt.setp(stemlines, linewidth=0.5) # Set linewidth for stemlines


plt.xlabel('Sum (X)')
plt.ylabel('Probability')
plt.title('PMF of Sum of Coin Toss and Die Roll')
plt.xticks(x)
plt.legend()
plt.grid(True)
plt.savefig('coin_die_pmf.png')  # Save the figure
plt.show()

