import numpy as np
import matplotlib.pyplot as plt

# Define the members of the council and their probabilities
members = np.array(['Woman', 'Man'])
total_members = 4 + 6  # 4 men, 6 women
probabilities = np.array([6 / total_members, 4 / total_members])  # Probability for women and men

# Plotting the stem plot
plt.stem(members, probabilities, basefmt=" ")
plt.xlabel("Council Member Type")
plt.ylabel("Probability")
plt.title("Probability of Selecting a Woman from the Council")
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.show()

