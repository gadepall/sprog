import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the shared object (.so) file
# Replace './code.so' with the actual path to your .so file
c_lib = ctypes.CDLL('./code.so')

# Define the numerical function signature
# void fun(double n, double t, double h, double *x, double *y, int steps)
c_lib.fun.argtypes = [
    ctypes.c_double,  # Initial population
    ctypes.c_double,  # Start time
    ctypes.c_double,  # Step size
    ctypes.POINTER(ctypes.c_double),  # Array for x (time)
    ctypes.POINTER(ctypes.c_double),  # Array for y (population)
    ctypes.c_int      # Number of steps
]
k=np.log(11/10)*(0.5)
# Wrapper for the C function
def numerical_solution_from_so():
    # Time step and total steps
    h = 0.1
    t_max = 14.55  # Calculate number of steps

    # Allocate arrays for t (x) and n (y)
    x = (ctypes.c_double * 100)()
    y = (ctypes.c_double * 100)()
    
    # Initial values
    n = ctypes.c_double(100000)
    t = ctypes.c_double(0)
    
    # Call the C function
    c_lib.fun(n, t, ctypes.c_double(h), x, y, ctypes.c_int(100))
    
    # Convert the results to numpy arrays
    return np.array(x[:100]), np.array(y[:100])

# Analytical expression N = 100000 * e^(t*k)
def analytical_solution(t):
    return 100000 * np.exp(t*k)

# Z-transform method (discretized version)
def z_transform_solution():
    h = 0.1
    n = 100000
    t = 0
    t_max = 14.55
    x = []
    z = []
    while t <= t_max:  # Iterate up to 14.55 hours
        x.append(t)
        z.append(n)
        n = n * (1 + h*k)  # Z-transform equivalent
        t = t + h
    return np.array(x), np.array(z)

# Time values for analytical solution
t_values = np.linspace(0, 14.55, 150)

# Get solutions
x_numerical, y_numerical = numerical_solution_from_so()
print(y_numerical)
x_z, z_values = z_transform_solution()
analytical_values = analytical_solution(t_values)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(t_values, analytical_values / 1e5, label='Theory', color='black', linestyle='-')  # Black line for theory
plt.scatter(x_numerical, y_numerical / 1e5, label='Sim1', color='red')  # Red dots for sim1
plt.plot(x_z, z_values / 1e5, label='Sim2', color='blue', linestyle='--')  # Blue dashed line for sim2
plt.xlabel('Time (t) [hours]')
plt.ylabel('N (in lakhs)')
plt.legend()
plt.grid()
plt.title('Bacteria Growth from $t=0$ to $t=14.55$ hours')
plt.savefig('plot.png')  # Save the figure as PNG
plt.show()