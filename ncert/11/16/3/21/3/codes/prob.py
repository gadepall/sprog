import numpy as np
import matplotlib.pyplot as plt

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

# Step 3: Simulate random selection for different sizes
random_choice1 = np.random.choice(groups, size=1000)  # Large sample for accuracy
random_choice2 = np.random.choice(groups, size=500000)
random_choice3 = np.random.choice(groups, size=10000000)
x = [1000,500000,10000000]

# Step 4: Calculate probability
onlyNSS_count1 = np.sum(random_choice1 == "NSS")
probability_onlyNSS1 =  onlyNSS_count1 / len(random_choice1)
onlyNSS_count2 = np.sum(random_choice2 == "NSS")
probability_onlyNSS2 =  onlyNSS_count2 / len(random_choice2)
onlyNSS_count3 = np.sum(random_choice3 == "NSS")
probability_onlyNSS3 =  onlyNSS_count3 / len(random_choice3)
# Output the results
print(f"Total students: {total_students}")
print(f"Distribution:")

print(f"  - Only NSS: {only_nss}")
print(f"  - Both NCC and NSS: {both_ncc_nss}")
print(f"  - NCC: {ncc_students}")
print(f"Simulated Probability of only  NSS size(1000): {probability_onlyNSS1:.5f}")
print(f"Simulated Probability of only  NSS size(500000): {probability_onlyNSS2:.5f}")
print(f"Simulated Probability of only  NSS size(1000000): {probability_onlyNSS3:.5f}")
y = [probability_onlyNSS1,probability_onlyNSS3,probability_onlyNSS3]
unique, counts = np.unique(random_choice1, return_counts=True)

# Plot stem plot 
plt.stem(x,y,label = "simulated probabilities for different sizes")
#plotting theoretical probability
x1 = [-5000,1000,500000,10000000]
plt.plot(x1,[0.13333,0.13333,0.13333,0.13333],label="theoretical probability",color='g',linestyle = '--')

# Labels and title
plt.xlabel("Category")
plt.ylabel("Frequency")
plt.legend()
plt.title("Distribution of Randomly Selected Students")
plt.show()
