import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared object (.so) file
dll = ctypes.CDLL('./ccode.so')

# Define argument and return types for the C function
dll.estimate_probability_complement.argtypes = [ctypes.c_float, ctypes.c_int]
dll.estimate_probability_complement.restype = ctypes.c_float

# Given probability P(B)
p_b = 0.48
n = 100000  # Number of simulations

# Call the C function to estimate P(B')
simulated_p_b_complement = dll.estimate_probability_complement(p_b, n)

# Theoretical probability P(B') = 1 - P(B)
theoretical_p_b_complement = 1 - p_b

# Plot both values
plt.stem(1, simulated_p_b_complement, linefmt="blue", markerfmt="bo", basefmt="black", label="Simulated P(B')")
plt.text(1, simulated_p_b_complement - 0.02, f"{simulated_p_b_complement:.3f}", 
         fontsize=10, horizontalalignment="center", verticalalignment="top")

plt.stem(2, theoretical_p_b_complement, linefmt="grey", markerfmt="go", basefmt="black", label="Theoretical P(B')")
plt.text(2, theoretical_p_b_complement - 0.02, f"{theoretical_p_b_complement:.3f}", 
         fontsize=10, horizontalalignment="center", verticalalignment="top")

# Labels and formatting
plt.xlabel("x")
plt.ylabel("Probability")
plt.xticks([1, 2], ["Simulated", "Theoretical"])
plt.ylim(0, 1)
plt.grid(True)
plt.legend()
plt.title("Comparison of Simulated and Theoretical P(B')")

# Save the plot
plt.savefig("../figs/pb_complement.png")
plt.show()

