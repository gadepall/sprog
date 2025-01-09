import ctypes
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import quad

# Load the shared object file
code = ctypes.CDLL('./code.so')

# Define the argument and return types for the trapezoidal function
code.trapezoidal.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
code.trapezoidal.restype = ctypes.c_double

# Define the Python functions for the curves
def f1(x):
    return np.sqrt(1 - (x - 1)**2)

def f2(x):
    return np.sqrt(1 - x**2)

# Define the difference function for the theoretical area
def difference(x):
    return f1(x) - f2(x)

# Integration bounds
x1, x2 = 0.0, 1.0

# Steps for the simulated area
steps = 1000

# Calculate the simulated area using the trapezoidal function in C
simulated_area = code.trapezoidal(x1, x2, steps)

# Calculate the theoretical area using numerical integration
theory_area, _ = quad(difference, x1, x2)

# Print the areas
print(f"Sim (C trapezoidal): {simulated_area}")
print(f"Theory (Python quad): {theory_area}")

# Generate x values for plotting
x_values = np.linspace(x1, x2, steps)
f1_values = f1(x_values)
f2_values = f2(x_values)

# Plot the curves and fill the area
plt.plot(x_values, f1_values, label=r'$f_1(x) = \sqrt{1 - (x-1)^2}$', color='blue')
plt.plot(x_values, f2_values, label=r'$f_2(x) = \sqrt{1 - x^2}$', color='green')
plt.fill_between(x_values, f1_values, f2_values, where=(f1_values > f2_values), 
                 interpolate=True, color='lightblue', alpha=0.5, label='Area')

# Add the simulated and theoretical area values as text
plt.text(0.5, 0.25, f'Sim: {simulated_area:.6f}', fontsize=10, color='red', ha='center')
plt.text(0.5, 0.15, f'Theory: {theory_area:.6f}', fontsize=10, color='black', ha='center')

# Add labels, legend, and show the plot
plt.xlabel('x')
plt.ylabel('y')
plt.title('Area Between Curves')
plt.legend()
plt.grid(True)
plt.show()

