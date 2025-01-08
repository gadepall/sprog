import matplotlib.pyplot as plt
from ctypes import CDLL, c_float

# Load the shared library
newvalues = CDLL('./b.so')

# Declare the functions from the shared library
x_n = newvalues.x_n
y_1 = newvalues.y_1
y_n = newvalues.y_n

# Set return types for the C functions
x_n.restype = c_float
y_1.restype = c_float
y_n.restype = c_float

# Initial conditions
x_0 = 0.0
y_0 = 0.0
dy_0 = 1.0  # Assumed first derivative
h = 0.1  # Increment in x

# Initialize variables for storing results
x_values = [x_0]
y_values = [y_0]
#computing x1
x1 = round(x_n(c_float(x_0), c_float(h)),3)
x_values.append(x1)
# Compute y1 using the first derivative
y1 = round(y_1(c_float(y_0), c_float(dy_0), c_float(h)),3)
y_values.append(y1)
# Compute subsequent y values using the finite difference method
for i in range(2, 100):  # Calculate up to y100
    x_i = round(x_n(c_float(x_values[-1]), c_float(h)),3)  # Update x
    y_i = round(y_n(c_float(y_values[-1]), c_float(y_values[-2]), c_float(h), c_float(x_values[-1])),3)
    x_values.append(x_i)
    y_values.append(y_i)
# Plot the results
plt.plot(x_values, y_values,color='blue', label='sim')
plt.xlabel("x")
plt.ylabel("y(x)")
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")
