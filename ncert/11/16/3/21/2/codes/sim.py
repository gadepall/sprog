import numpy as np

# Given data
total_students = 60       # Total number of students
ncc_students = 30         # Students who opted for NCC
nss_students = 32         # Students who opted for NSS
both_ncc_nss = 24         # Students who opted for both NCC and NSS

# Step 1: Calculate the number of students in only NCC, only NSS, and neither
only_ncc = ncc_students - both_ncc_nss  # Students in only NCC
only_nss = nss_students - both_ncc_nss  # Students in only NSS
neither = total_students - (only_ncc + only_nss + both_ncc_nss)  # Students in neither

# Step 2: Define the distribution
# Assigning labels for the groups
groups = (
    ["NCC"] * only_ncc +        # Only NCC
    ["NSS"] * only_nss +        # Only NSS
    ["Both"] * both_ncc_nss +   # Both NCC and NSS
    ["Neither"] * neither       # Neither NCC nor NSS
)

# Step 3: Simulate random selection
random_choice = np.random.choice(groups, size=1000000)  # Large sample for accuracy

# Step 4: Calculate probability
neither_count = np.sum(random_choice == "Neither")
probability_neither = neither_count / len(random_choice)

# Output the results
print(f"Total students: {total_students}")
print(f"Distribution:")
print(f"  - Only NCC: {only_ncc}")
print(f"  - Only NSS: {only_nss}")
print(f"  - Both NCC and NSS: {both_ncc_nss}")
print(f"  - Neither: {neither}")
print(f"Simulated Probability of neither NCC nor NSS: {probability_neither:.5f}")

