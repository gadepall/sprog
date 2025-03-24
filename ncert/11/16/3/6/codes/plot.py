import ctypes
import matplotlib.pyplot as plt
import numpy as np

# Load the shared library (make sure to build the C shared library correctly)
council_sim = ctypes.CDLL('./code.so')

# Set up the C function's argument and return types
council_sim.simulate_selection.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_int)]

# Parameters
trials = 1000000  # Number of trials for better accuracy
results = (ctypes.c_int * 2)()  # Results array for women (index 0) and men (index 1)

# Call the C function
council_sim.simulate_selection(trials, results)

# Extract results and calculate probabilities
woman_count = results[0]  # Count of women selected
man_count = results[1]  # Count of men selected

# Probabilities
woman_probability = woman_count / trials
man_probability = man_count / trials

# Categories for plotting
categories = ['Women', 'Men']
probabilities = [woman_probability, man_probability]

# Plot the probability distribution
plt.bar(categories, probabilities, color='skyblue')
plt.xlabel("Council Member Type")
plt.ylabel("Probability")
plt.ylim(0, 1)  # Set y-axis to range from 0 to 1 for clarity
plt.title("Probability of Selecting a Council Member")
plt.show()

