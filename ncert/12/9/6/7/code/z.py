import ctypes
import os
import matplotlib.pyplot as plt
import numpy as np

# Provide the full path to the shared object if it's not in the same directo	ry
lib = ctypes.CDLL('/home/malakala/Desktop/GVV-SIR-MATRIXTHEORY/EE1030/9.6.7/code/code.so')

# Define the argument types and return type for the function
lib.finite_difference.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_char_p]
lib.finite_difference.restype = None

# Prepare the input parameters
x0 = 2.718282     # Initial value of x
y0 = -2 / 2.718282    # Initial value of y
x_end = 200.0   # End value of x
h = 0.1    # Step size
filename = b"output.dat"  # Output file (needs to be bytes for C)

# Call the finite_difference function
lib.finite_difference(x0, y0, x_end, h, filename)

# Check if the file was created
if os.path.exists("output.dat"):
    # Open the file and read data
    x_values = []
    y_values = []

    with open("output.dat", "r") as file:
        # Skip the header line
        next(file)
        # Read each line and extract x, y values
        for line in file:
            # Split the line into x and y
            x, y = map(float, line.split())
            x_values.append(x)
            y_values.append(y)

    # Plot the data using matplotlib with smaller points
    plt.plot(x_values, y_values, label="sim", marker='o', markersize=3, color='blue')
    
    # Create x values for the second function y = x * asin(x) + sqrt(1 - x^2)
    x_vals = np.linspace(2.718282,200, 1000)
    y_vals = (-2)/((x_vals*np.log(x_vals)*np.log(x_vals)))-0.05

    # Plot the second function
    plt.plot(x_vals, y_vals, label=r'$theory$', color='red')

    # Customize the plot
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)
    plt.legend()
    plt.show()

else:
    print("The file was not created.")

