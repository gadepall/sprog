import ctypes  #ctypes provides a way to create and manage C-compatible arrays directly in Python.
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library (.so file generated from C code)
output = ctypes.CDLL("./output.so")
#When we load a shared library using ctypes.CDLL, we get a Python object that corresponds to the C library. This allows us to call the functions defined in the C library.
#output is the variable in which we store this 
# Prepare arrays for numerical solution (from the C code)
N = 10000
x_values = (ctypes.c_double * N)()  #ctypes.c_double represents the C data type double (a floating-point number in C).
y_values = (ctypes.c_double * N)()

# Call the C function to populate x_values and y_values
output.calculate_values(x_values, y_values)

# Convert C arrays into NumPy arrays for easy plotting
x_values_np = np.array([x_values[i] for i in range(N)])
y_values_np = np.array([y_values[i] for i in range(N)]) #This code is converting the C-style arrays (x_values and y_values) created using ctypes into NumPy arrays 

# Define the theoretical solution using the integrating factor method
def theoretical_solution(x):
    return x*x*x/4+1/x 

# Generate x values for the theoretical solution (avoiding pi/2)
x_theory = np.linspace(1.0,2,500)
y_theory = theoretical_solution(x_theory)

# Plot both numerical and exact solutions on the same graph
plt.figure(figsize=(10, 7))
plt.plot(x_values_np, y_values_np, 'yo', label="simulation", markersize=5)
plt.plot(x_theory, y_theory, '-', label="theory")
plt.xlabel('x-axis')
plt.ylabel('y-axis')
plt.grid(True)
plt.legend()
plt.savefig('combined_plot.pdf')
plt.show()



