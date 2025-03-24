import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./code.so')  # Make sure the path is correct

# Define the return type of the function (double in this case)
lib.Prob.restype = ctypes.c_double

# Generate the random variable distribution
num_trials = 10000  # You can use the same number of trials as in the C code
probabilities = []

for _ in range(num_trials):
    prob = lib.Prob()  # Call the C function
    probabilities.append(prob)

# Convert the list to a numpy array for easier handling
probabilities = np.array(probabilities)

# Plot the distribution of the random variable
plt.hist(probabilities, bins=50, density=True, alpha=0.6, color='g')
plt.title('Random Variable Distribution')
plt.xlabel('Probability')
plt.ylabel('Frequency')
plt.savefig('plot.png')
plt.show()


