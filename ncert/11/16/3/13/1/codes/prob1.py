import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
dll = ctypes.CDLL('./prob1.so')

# Setting argument types for the function
dll.union_probability.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.union_probability.restype = ctypes.POINTER(ctypes.c_float)

# Setting argument type for freeMemory function
dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.c_float)]
dll.freeMemory.restype = None

# Theoretical probabilities
p_A = 1/3
p_B = 1/5
p_AB = 1/15
p_A_or_B = p_A + p_B - p_AB  # (1/3 + 1/5 - 1/15)

# Number of trials
trials = 100000

# Call the C function to get the computed values
computed_probabilities = dll.union_probability(ctypes.c_float(p_A), ctypes.c_float(p_B), ctypes.c_float(p_AB), ctypes.c_int(trials))

# Converting the result by c code to a Python list
computed_values = [computed_probabilities[i] for i in range(4)]

# Free the memory allocated in C
dll.freeMemory(computed_probabilities)

# X-axis labels
labels = ["Pr(A)", "Pr(B)", "Pr(AB)", "Pr(A+B)"]
x = np.arange(len(labels))

# Theoretical values 
theoretical_values = [p_A, p_B, p_AB, p_A_or_B]

plt.figure(figsize=(10, 6))

# Offset the computed values to the right (for separation)
offset = 0.1  
x_theoretical = x - offset
x_computed = x + offset

# Plot theoretical values (blue)
plt.stem(x_theoretical, theoretical_values, linefmt='b-', markerfmt='bo', basefmt="k-", label="Theory")
# Plot computed values (brown)
plt.stem(x_computed, computed_values, linefmt='brown', markerfmt='ro', basefmt="k-", label="Sim")

plt.xticks(x, labels)
plt.xlabel("Event")
plt.ylabel("Probability")
plt.grid(True)

# Annotate the values on the plot
for i, value in enumerate(theoretical_values):
    plt.annotate(f'{value:.3f}', (x_theoretical[i], value), textcoords="offset points", xytext=(0, 10), ha='center')
for i, value in enumerate(computed_values):
    plt.annotate(f'{value:.3f}', (x_computed[i], value), textcoords="offset points", xytext=(0, 10), ha='center')

plt.legend()
plt.tight_layout()
plt.show()

