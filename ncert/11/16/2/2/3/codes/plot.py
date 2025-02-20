import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
dice_lib = ctypes.CDLL("./dice_sim.so")

# Define function argument and return types
dice_lib.roll_dice.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
dice_lib.filter_multiples_of_3.argtypes = [
    ctypes.POINTER(ctypes.c_int), ctypes.c_int,
    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)
]
dice_lib.count_occurrences.argtypes = [
    ctypes.POINTER(ctypes.c_int), ctypes.c_int,
    ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)
]

# Simulation parameters
num_trials = 1000000

# Allocate memory for dice rolls
dice_rolls = (ctypes.c_int * num_trials)()
filtered_rolls = (ctypes.c_int * num_trials)()
filtered_size = ctypes.c_int()

# Step 1: Roll the dice
dice_lib.roll_dice(dice_rolls, num_trials)

# Step 2: Filter multiples of 3
dice_lib.filter_multiples_of_3(dice_rolls, num_trials, filtered_rolls, ctypes.byref(filtered_size))

# Convert filtered results to a NumPy array
filtered_results = np.array(filtered_rolls[:filtered_size.value])

# Step 3: Count occurrences of 3 and 6
count_3 = ctypes.c_int()
count_6 = ctypes.c_int()
dice_lib.count_occurrences(filtered_rolls, filtered_size, ctypes.byref(count_3), ctypes.byref(count_6))

# Compute empirical probabilities
total_filtered = filtered_size.value
pmf_simulated = {
    3: count_3.value / total_filtered,
    6: count_6.value / total_filtered
}

# Theoretical PMF
pmf_theoretical = {3: 0.5, 6: 0.5}

# Print results
print("Simulated PMF:", pmf_simulated)
print("Theoretical PMF:", pmf_theoretical)

#plot
x_values = [3, 6]  # Outcomes that satisfy the condition
y_values_sim = [pmf_simulated[3], pmf_simulated[6]]  # Simulated PMF values
y_values_theo = [pmf_theoretical[3], pmf_theoretical[6]]  # Theoretical PMF values

# Create figure
plt.figure(figsize=(6, 4))

# Simulated PMF using plt.stem()
plt.stem(x_values, y_values_sim, linefmt='b-', markerfmt='bo', basefmt=" ", label="Simulated")

# Theoretical PMF using plt.scatter()
plt.scatter(x_values, y_values_theo, color='red', label="Theoretical",zorder=3)

# Fix x-axis ticks
plt.xticks([1, 2, 3, 4, 5, 6])
plt.xlabel("Die Outcome")
plt.ylabel("Probability")
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")

