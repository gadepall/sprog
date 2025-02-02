import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# Linking the .so file to this Python file
dll = ctypes.CDLL('./c.so')

# Setting the argument types and the return type for the functions in the C code
dll.union_probability.argtypes = [ctypes.c_float]*3 + [ctypes.c_int]
dll.union_probability.restype = ctypes.POINTER(ctypes.c_float)

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.c_float)]
dll.freeMemory.restype = None

# Setting the probabilities given in the question
p_e = 1/4  # P(E) = 0.25
p_f = 1/2  # P(F) = 0.50
p_e_f = 1/8  # P(E ∩ F) = 0.125
n = 100000  # Number of simulations

# Storing the array of simulated coordinates in the 1D array 'probability'
probability = dll.union_probability(p_e, p_f, p_e_f, n)

# Plotting the simulated probabilities
plt.plot([6.5, 8.5], [probability[0] + probability[1] - probability[2], probability[0] + probability[1] - probability[2]], linestyle="--", color="black")
plt.scatter(7.7, probability[0] + probability[1] - probability[2], color="green")
plt.stem(1, probability[0], linefmt="blue", label="P(E)")
plt.text(0.65, probability[0] - 0.05, f"{probability[0]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(3, probability[1], linefmt="red", label="P(F)")
plt.text(2.65, probability[1] - 0.05, f"{probability[1]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(5, probability[2], linefmt="green", label="P(E ∩ F)")
plt.text(4.65, probability[2] - 0.05, f"{probability[2]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(7, probability[3], linefmt="orange", label="P(E ∪ F)")
plt.text(6.65, probability[3] - 0.05, f"{probability[3]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(7.8, probability[0] + probability[1], linefmt="red")
plt.stem(7.8, probability[0], linefmt="blue")
x_coord = [7.7, 7.7]
y_coord = [probability[0] + probability[1] - probability[2], probability[0] + probability[1]]
plt.plot(x_coord, y_coord, color="green")

# Plotting the theoretical probabilities
plt.stem(1.2, 0.25, linefmt="grey")
plt.text(1.5, 0.25 - 0.05, "0.25", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(3.2, 0.50, linefmt="grey")
plt.text(3.5, 0.50 - 0.05, "0.50", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(5.2, 0.125, linefmt="grey")
plt.text(5.5, 0.125 - 0.05, "0.125", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(7.2, 0.625, linefmt="grey")
plt.text(7.5, 0.625 - 0.05, "0.625", fontsize=10, horizontalalignment="center", verticalalignment="top")

# Freeing the used memory
dll.freeMemory(probability)

plt.xlabel("x")
plt.ylabel("y")
plt.xlim(0, 9)
plt.grid(True)
plt.legend()
ax = plt.gca()
ax.set_yticks(np.arange(0, 1.1, 0.1))
ax.xaxis.set_tick_params(labelbottom=False)
plt.show()

