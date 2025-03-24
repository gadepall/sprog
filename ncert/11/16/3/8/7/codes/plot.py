import ctypes
import matplotlib.pyplot as plt

# Load the C shared library
c_lib = ctypes.CDLL('./code.so')  # Replace with the correct path to your .so file

# Define the results array (4 elements for 0 to 3 tails)
results = (ctypes.c_int * 4)(0, 0, 0, 0)
num_trials = 1000000

# Call the simulate_coin_tosses function from the C library
c_lib.simulate_coin_tosses(num_trials, results)

# Convert results to Python list
results = list(results)

# Calculate PMF
pmf = [count / num_trials for count in results]

# Calculate CDF
cdf = []
cumulative = 0
for prob in pmf:
    cumulative += prob
    cdf.append(cumulative)

# Plot PMF
plt.figure(figsize=(10, 6))
plt.stem(range(4))
plt.title("Probability Mass Function (PMF)")
plt.xlabel("Number of Tails")
plt.ylabel("Probability")
plt.xticks(range(4), labels=["0 Tails", "1 Tail", "2 Tails", "3 Tails"])
plt.grid(True)
plt.show()

# Plot CDF
plt.figure(figsize=(10, 6))
plt.stem(range(4))
plt.title("Cumulative Distribution Function (CDF)")
plt.xlabel("Number of Tails")
plt.ylabel("Cumulative Probability")
plt.xticks(range(4), labels=["0 Tails", "1 Tail", "2 Tails", "3 Tails"])
plt.grid(True)
plt.show()

