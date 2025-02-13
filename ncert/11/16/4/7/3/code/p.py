import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Linking the .so file generated from the C code
dll = ctypes.CDLL('./lib.so')

# Setting argument types and return type for the C function
dll.compute_probability.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.compute_probability.restype = ctypes.c_float

# Given probabilities
p_a = 0.54
p_b = 0.69
p_a_p_b = 0.35
n = 1000000  # Large n for better accuracy

# Compute P(A ∩ B') using the C function
p_a_b_comp = dll.compute_probability(p_a, p_b, p_a_p_b, n)

# Compute theoretical probability
p_a_b_comp_theory = p_a - p_a_p_b

# Plot results
plt.stem(1, p_a, linefmt="blue", label="P(A)")
plt.text(0.65, p_a - 0.05, f"{p_a:.3f}", fontsize=10, horizontalalignment="center")

plt.stem(3, p_b, linefmt="red", label="P(B)")
plt.text(2.65, p_b - 0.05, f"{p_b:.3f}", fontsize=10, horizontalalignment="center")

plt.stem(5, p_a_p_b, linefmt="green", label="P(A ∩ B)")
plt.text(4.65, p_a_p_b - 0.05, f"{p_a_p_b:.3f}", fontsize=10, horizontalalignment="center")

plt.stem(7, p_a_b_comp, linefmt="orange", label="P(A ∩ B') (Simulated)")
plt.text(6.65, p_a_b_comp - 0.05, f"{p_a_b_comp:.3f}", fontsize=10, horizontalalignment="center")

plt.stem(7.2, p_a_b_comp_theory, linefmt="grey", label="P(A ∩ B') (Theoretical)")
plt.text(7.5, p_a_b_comp_theory - 0.05, f"{p_a_b_comp_theory:.3f}", fontsize=10, horizontalalignment="center")

# Formatting
plt.xlabel("Events")
plt.ylabel("Probability")
plt.xlim(0, 8)
plt.ylim(0, 1)
plt.grid(True)
plt.legend()
ax = plt.gca()
ax.set_yticks(np.arange(0, 1.1, 0.1))
ax.xaxis.set_tick_params(labelbottom=False)

# Save and show the plot
plt.savefig("../figure/fig.png")
plt.show()
