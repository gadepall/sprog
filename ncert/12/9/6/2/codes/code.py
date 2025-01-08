import math
import ctypes
import matplotlib.pyplot as plt

# Loading the .so file
lib = ctypes.CDLL('./funcs.so')

# Definations of return types and argument types in C code
lib.vals.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_int]
lib.vals.restype = None

lib.dydx.argtypes = [ctypes.c_float, ctypes.c_float]
lib.dydx.restype = ctypes.c_float

# Set the initial values and parameters
x = ctypes.c_float(0.0) 
y = ctypes.c_float(1.0)
n = 5000
h = 0.0002

# Creating arrays to store the results for plotting
x_vals = []
y_vals = []
theory_values = []

# Generate values using the C function
for i in range(n):
    x_vals.append(float(x.value))
    y_vals.append(float(y.value))

    # Compute theoretical values for y using the known solution of the ODE
    theory_y = math.exp(-2*x.value)
    theory_values.append(theory_y)

    # Call the C function to update x and y
    lib.vals(ctypes.byref(x), ctypes.byref(y), 1)
#plotting
sim_line, = plt.plot(x_vals, y_vals, label="simulation", color='midnightblue')
theory_line, = plt.plot(x_vals, theory_values, label="theoretical", color='chartreuse', linestyle='--')
plt.xlabel("x-axis")
plt.ylabel("y-axis")
# Customize axis spines for thick black axes
ax = plt.gca()  # Get the current axes
ax.spines['bottom'].set_color('black')  # Bottom axis
ax.spines['bottom'].set_linewidth(2)    # Set thickness
ax.spines['left'].set_color('black')    # Left axis
ax.spines['left'].set_linewidth(2)      # Set thickness
# Customize tick parameters for thicker black ticks
ax.tick_params(axis='both', colors='black', width=2, length=6)
plt.legend(handles=[sim_line, theory_line])
plt.grid(True)
plt.show()
