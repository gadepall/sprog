import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared library
lib = ctypes.CDLL('./lib.so')

# Define the C function prototypes
lib.y_values.argtypes = [ctypes.c_double, ctypes.c_double]
lib.y_values.restype = ctypes.c_double

# Parameters
y_0 = 3.0
h = 0.1

# Initialise variables
x_itr = [0]
y_itr = [y_0]

# Iteratively compute y values
y = y_0
for i in range(1,100):
    x = i*h
    y = lib.y_values(y, h)
    x_itr.append(x)
    y_itr.append(y)

# Theoretical solution
def theo_result(x):
    return (63*x + 27)**(1/3)

# Calculating values
x_theory = np.linspace(0, 10, 1000)  # Range of x for plotting
y_theory = theo_result(x_theory)


# Plot the results
plt.plot(x_itr, y_itr, label = "Sim" , color="red")
plt.plot(x_theory, y_theory, label = "Theory" , color="green")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()
plt.grid(True)
plt.savefig('../figs/fig.png')
plt.show()
