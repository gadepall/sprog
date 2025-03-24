import ctypes
import matplotlib.pyplot as plt
import numpy as np

# Load the shared library (make sure to build the C shared library correctly)
student_sim = ctypes.CDLL('./code.so')

# Set up the C function's argument and return types
student_sim.simulate_selection.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_int)]

# Parameters
trials = 1000000  # Number of trials for better accuracy
results = (ctypes.c_int * 2)()  # Results array for NCC/NSS (index 0) and others (index 1)

# Call the C function
student_sim.simulate_selection(trials, results)

# Extract results and calculate probabilities
ncc_or_nss_count = results[0]  # Count of students in NCC or NSS selected
others_count = results[1]      # Count of students not in NCC or NSS selected

# Probabilities
ncc_or_nss_probability = ncc_or_nss_count / trials
others_probability = others_count / trials

# Categories for plotting
categories = ['NCC or NSS', 'Others']
probabilities = [ncc_or_nss_probability, others_probability]

# Plot the probability distribution
plt.bar(categories, probabilities, color='skyblue')
plt.xlabel("Student Category")
plt.ylabel("Probability")
plt.ylim(0, 1)  # Set y-axis to range from 0 to 1 for clarity
plt.title("Plot of the given information")
plt.show()

