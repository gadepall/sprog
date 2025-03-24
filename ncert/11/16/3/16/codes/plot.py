import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
prob_lib = ctypes.CDLL('./monte_carlo.so')

# Define function prototypes for the C function
prob_lib.compute_probabilities.restype = ctypes.POINTER(ctypes.c_float * 2)
prob_lib.compute_probabilities.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_int]

# Function to plot the theoretical and simulated probabilities
def plot_probabilities(p_A, p_B, n):
    # Call the compute_probabilities function from the C shared library
    result = prob_lib.compute_probabilities(ctypes.c_float(p_A), ctypes.c_float(p_B), ctypes.c_int(n))
    
    # Extract the results from the returned pointer
    simulated_probs = np.array(result.contents)
    
    # Theoretical probabilities
    theoretical_probs = np.array([p_A, p_B])
    
    # Labels for the probabilities
    labels = ['P(not E+ not F)', 'P(EF)']
    x = np.arange(2)

    # Create a stem plot for the theoretical and simulated probabilities
    plt.figure(figsize=(6, 5))
    markerline1, stemlines1, baseline1 = plt.stem(x - 0.1, theoretical_probs, label='Theory', basefmt=" ", use_line_collection=True)
    markerline2, stemlines2, baseline2 = plt.stem(x + 0.1, simulated_probs, label='Simulated', basefmt=" ", use_line_collection=True)

    # Customize appearance
    markerline1.set_markerfacecolor('blue')
    markerline1.set_markeredgecolor('blue')
    stemlines1.set_color('blue')
    markerline2.set_markerfacecolor('red')
    markerline2.set_markeredgecolor('red')
    stemlines2.set_color('red')

    # Add annotations above each stem
    for i in range(2):
        plt.text(x[i] - 0.1, theoretical_probs[i] + 0.02, f'{theoretical_probs[i]:.3f}', 
                 horizontalalignment='center', verticalalignment='bottom', color='blue', fontsize=10)
        plt.text(x[i] + 0.1, simulated_probs[i] + 0.02, f'{simulated_probs[i]:.3f}', 
                 horizontalalignment='center', verticalalignment='bottom', color='red', fontsize=10)

    # Set y-axis limit
    plt.ylim(0, 1)
    plt.xticks(x, labels)
    plt.ylabel('Probability')
    plt.legend()
    plt.savefig("../plots/plot.png")
    plt.show()

# Example usage
p_A = 0.25
p_B = 0.75
n = 100000
plot_probabilities(p_A, p_B, n)

