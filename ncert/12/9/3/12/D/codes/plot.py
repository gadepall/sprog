import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Compile the C code into a shared library (.so) file:
# gcc -shared -o ode_solver.so -fPIC ode_solver.c

# Load the shared library
ode_solver = ctypes.CDLL('./func.so')

# Set up the function signature
ode_solver.solve_ode.argtypes = [ctypes.c_double, ctypes.POINTER(ctypes.c_double)]

# Parameters
h = 0.1  # Step size
n = 51   # Number of points (x from 0 to 5)

# Initialize y array (y[0] = initial value)
y = np.zeros(n + 1, dtype=np.double)
y[0] = 1.0  # Initial condition y(0) = 1

# Convert the y array to a ctypes pointer
y_ctypes = y.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the C function
ode_solver.solve_ode(h, y_ctypes)

# Extract x values
x = np.linspace(0, n * h, n + 1)

# Plot the results
plt.figure(figsize=(8, 6))
plt.plot(x, y, label="y(x)", marker='o')
plt.title("plot")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.savefig("../figure/fig.png")
plt.show()

