import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Linking the .so file to this Python file
dll = ctypes.CDLL('./test.so')

# Setting the argument types and the return type for the functions in the C code
dll.prob.argtypes = None
dll.prob.restype = ctypes.c_double

# Generate the first plot: Probability Mass Function (PMF)
plt.figure(1)  # Create the first figure
plt.stem([1], [dll.prob()], linefmt="blue", markerfmt="bo", basefmt=" ", label="P({X = 1})")
plt.stem([3], [1 - dll.prob()], linefmt="red", markerfmt="ro", basefmt=" ", label="P({X = 0})")
plt.text(1, dll.prob() + 0.05, f"{dll.prob():.6f}", verticalalignment="top", horizontalalignment="center")
plt.text(3, 1 - dll.prob() + 0.05, f"{1 - dll.prob():.6f}", verticalalignment="top", horizontalalignment="center")

# Customize the first plot
plt.xlabel("x")
plt.ylabel("Probability")
plt.xlim(-1, 4)
plt.grid(True)
plt.legend()
ax = plt.gca()
ax.set_yticks(np.arange(0, 1.1, 0.1))
ax.xaxis.set_tick_params(labelbottom=False)

# Save the first plot
plt.savefig("../figs/pmf.png")
plt.show()

# Generate the second plot: Cumulative Distribution Function (CDF)
plt.figure(2)  # Create the second figure
X = np.array([0, 1, 3])
F = np.array([0, dll.prob(), 1])  # Cumulative probabilities

# Plot the CDF
plt.step(X, F, where="post", label="CDF")

# Customize the second plot
plt.xlabel("x")
plt.ylabel("Cumulative Probability")
plt.xlim(-1, 4)
plt.grid(True)
plt.legend()
ax = plt.gca()
ax.set_yticks(np.arange(0, 1.1, 0.1))
ax.xaxis.set_tick_params(labelbottom=False)

# Save the second plot
plt.savefig("../figs/cdf.png")
plt.show()
