import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# Linking the .so file to this Python file
dll = ctypes.CDLL('./code.so')

# Setting the argument types and the return type for the functions in the C code
dll.union_probability.argtypes = [ctypes.c_float] * 3 + [ctypes.c_int]
dll.union_probability.restype = ctypes.POINTER(ctypes.c_float)

dll.freeMemory.argtype = ctypes.POINTER(ctypes.c_float)
dll.freeMemory.restype = None

# Setting the probabilities given in the question
p_a = 0.8
p_b = 0.7
p_a_union_b = 0.95
p_a_inter_b = p_a + p_b - p_a_union_b  # Calculate P(A ∩ B)
n = 100000

# Storing the array of simulated coordinates in the 1D array 'probability'
probability = dll.union_probability(p_a, p_b, p_a_inter_b, n)

# Plotting the simulated probabilities
plt.plot([6.5, 8.5], [probability[0] + probability[1] - probability[2], probability[0] + probability[1] - probability[2]], linestyle="--", color="black")
plt.scatter(7.7, probability[0] + probability[1] - probability[2], color="green")
plt.stem(1, probability[0], linefmt="blue", label="P(A)")
plt.text(0.65, probability[0] - 0.05, f"{probability[0]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(3, probability[1], linefmt="red", label="P(B)")
plt.text(2.65, probability[1] - 0.05, f"{probability[1]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(5, probability[2], linefmt="green", label="P(A ∩ B)")
plt.text(4.65, probability[2] - 0.05, f"{probability[2]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(7, probability[3], linefmt="orange", label="P(A ∪ B)")
plt.text(6.65, probability[3] - 0.05, f"{probability[3]:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(7.8, probability[0] + probability[1], linefmt="red")
plt.stem(7.8, probability[0], linefmt="blue")
x_coord = [7.7, 7.7]
y_coord = [probability[0] + probability[1] - probability[2], probability[0] + probability[1]]
plt.plot(x_coord, y_coord, color="green")

# Plotting the theoretical probabilities
plt.stem(1.2, 0.8, linefmt="grey")
plt.text(1.5, 0.8 - 0.05, "0.8", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(3.2, 0.7, linefmt="grey")
plt.text(3.5, 0.7 - 0.05, "0.7", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(5.2, p_a_inter_b, linefmt="grey")
plt.text(5.5, p_a_inter_b - 0.05, f"{p_a_inter_b:.3f}", fontsize=10, horizontalalignment="center", verticalalignment="top")
plt.stem(7.2, 0.95, linefmt="grey")
plt.text(7.5, 0.95 - 0.05, "0.95", fontsize=10, horizontalalignment="center", verticalalignment="top")

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
plt.savefig("../figs/simulated.png")
plt.show()
