import numpy as np
import matplotlib.pyplot as plt

# Define the equations for plotting
def equation1(x):
    return (4 - 60 * x) / 240  # Rearranging 60a + 240b = 4 to b = (4 - 60a)/240

def equation2(x):
    return ((25/6) - 100 * x) / 200  # Rearranging 100a + 200b = 25/6 to b = ((25/6) - 100a)/200

# Generate x values for plotting
x = np.linspace(0, 0.1, 500)  # Adjusted range to fit expected values of a and b

# Plot the equations
plt.plot(x, equation1(x), label='60a + 240b = 4', color='blue')
plt.plot(x, equation2(x), label='100a + 200b = 25/6', color='orange')

# Solution point (a, b) from LU decomposition
solution_a = 1 / 60  # a = 1 / speed of train
solution_b = 1 / 80  # b = 1 / speed of bus
plt.scatter(solution_a, solution_b, color='red', label=f'Solution: (a={solution_a:.4f}, b={solution_b:.4f})')

# Add labels and grid
plt.xlabel('a = 1 / Speed of Train')
plt.ylabel('b = 1 / Speed of Bus')
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.axvline(0, color='black', linewidth=0.5, linestyle='--')

# Add legend
plt.legend()
plt.grid(True)
plt.title("Graphical Solution of LU Decomposition System")
plt.show()

