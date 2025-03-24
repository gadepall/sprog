import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
lib = ctypes.CDLL('./combinatorial_probability.so')

# Define return types
lib.count_valid_numbers.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]

# Prepare integer variables
total = ctypes.c_int()
div_by_5 = ctypes.c_int()

# Call the C function
lib.count_valid_numbers(ctypes.byref(total), ctypes.byref(div_by_5))

# Compute probability
probability = div_by_5.value / total.value

# Data for visualization
x_values = np.arange(2)
y_values = [total.value, div_by_5.value]

# Stem plot
plt.figure(figsize=(8, 4))
plt.stem(x_values, y_values, use_line_collection=True)
plt.xticks([0, 1], ['Total Valid Numbers', 'Divisible by 5'])
plt.ylabel('Count')
#plt.title('Stem Plot of Valid Numbers')
plt.savefig("../figs/fig1.png")
plt.show()

# Step plot
plt.figure(figsize=(8, 4))
plt.step(x_values, y_values, where='mid', linestyle='-', marker='o')
plt.xticks([0, 1], ['Total Valid Numbers', 'Divisible by 5'])
plt.ylabel('Count')
#plt.title('Step Plot of Valid Numbers')
plt.savefig("../figs/fig2.png")
plt.show()

print(f'Probability of forming a number divisible by 5: {probability:.4f}')

