import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared object (.so) file
# Replace './code.so' with the actual path to your .so file
c_lib = ctypes.CDLL('./code.so')

# Define the numerical function signature
# void fun(double p, double t, double h, double *x, double *y)
c_lib.fun.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, 
                      ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]

# Wrapper for the C function
def numerical_solution_from_so():
    # Allocate arrays for t (x) and p (y)
    x = (ctypes.c_double * 100)()
    y = (ctypes.c_double * 100)()
    
    # Initial values
    p = ctypes.c_double(1000)
    t = ctypes.c_double(0)
    h = ctypes.c_double(0.1)
    
    # Call the function
    c_lib.fun(p, t, h, x, y)
    
    # Convert the results to numpy arrays
    return np.array(x), np.array(y)

# Analytical expression P = 1000 * e^(t/20)
def analytical_solution(t):
    return 1000 * np.exp(t / 20)

# Z-transform method (discretized version)
def z_transform_solution():
    h = 0.1
    p = 1000
    t = 0
    x = []
    z = []
    for _ in range(100):
        x.append(t)
        z.append(p)
        p = p * (1 + h / 20)  # Z-transform equivalent
        t = t + h
    return np.array(x), np.array(z)

# Time values for analytical solution
t_values = np.linspace(0, 10, 100)

# Get solutions
x_numerical, y_numerical = numerical_solution_from_so()
x_z, z_values = z_transform_solution()
analytical_values = analytical_solution(t_values)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(t_values, analytical_values, label='theory', color='black', linestyle='-')  # Black line for theory
plt.scatter(x_numerical, y_numerical, label='sim1', color='red')  # Red dots for sim1
plt.plot(x_z, z_values, label='sim2', color='blue', linestyle='--')  # Blue dashed line for sim2
plt.xlabel('Time (t)')
plt.ylabel('P')
plt.legend()
plt.grid()
plt.savefig('plot.png')  # Save the figure as PNG
plt.show()

