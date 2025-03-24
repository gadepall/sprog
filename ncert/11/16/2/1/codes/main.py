import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the compiled C shared library
prob_lib = ctypes.CDLL('./func.so')

# Define output variables
SIDES = 6
pmf = (ctypes.c_double * SIDES)()
p_e = ctypes.c_double()
p_f = ctypes.c_double()
p_e_and_f = ctypes.c_double()

# Call the C function
prob_lib.calculate_probabilities(pmf, ctypes.byref(p_e), ctypes.byref(p_f), ctypes.byref(p_e_and_f))

# Convert results to Python variables
PMF_VALUES = np.array(pmf[:])
P_E = p_e.value
P_F = p_f.value
P_E_AND_F = p_e_and_f.value

# Print probability results
print(f"P(E) = {P_E:.4f} (Die shows 4)")
print(f"P(F) = {P_F:.4f} (Die shows an even number)")
print(f"P(E ∩ F) = {P_E_AND_F:.4f} (Die shows 4 and is even)")

# Check if events are mutually exclusive
if P_E_AND_F > 0:
    print("E and F are NOT mutually exclusive.")
else:
    print("E and F are mutually exclusive.")

# Define x-axis labels (die outcomes)
x = np.arange(1, SIDES + 1)

# Create a figure with two subplots
fig, ax = plt.subplots(1, 2, figsize=(12, 5))

# Plot the PMF of rolling a die
ax[0].stem(x, PMF_VALUES, basefmt=" ", use_line_collection=True)
ax[0].set_title("PMF of Rolling a Die")
ax[0].set_xlabel("Die Outcome")
ax[0].set_ylabel("Probability")
ax[0].set_xticks(x)
ax[0].grid()

# Plot the probabilities of E, F, and E ∩ F
labels = ["P(E)", "P(F)", "P(E ∩ F)"]
values = [P_E, P_F, P_E_AND_F]
ax[1].bar(labels, values, color=['blue', 'green', 'red'])
ax[1].set_title("Probabilities of Events E, F, and E ∩ F")
ax[1].set_ylabel("Probability")
ax[1].set_ylim(0, 1)

# Show plots
plt.tight_layout()
plt.savefig('../figs/fig.png')
plt.show()

