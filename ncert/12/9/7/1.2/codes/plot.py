import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the compiled C library
lib = ctypes.CDLL('./runge_kutta.so')  # Replace with the actual path to the compiled shared library

# Define the argument and return types for the `runge_kutta` function
lib.runge_kutta.argtypes = [
    ctypes.c_double,  # x0
    ctypes.c_double,  # y0
    ctypes.c_double,  # v0
    ctypes.c_double,  # x_end
    ctypes.c_double,  # h
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # x_array
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # y_array
    ctypes.POINTER(ctypes.c_int),  # size
]

# Define initial conditions and parameters
x0 = 0.0
y0 = 0.0
v0 = 4
x_end = 5.0
h = 0.01

# Prepare pointers to store the results
x_array_ptr = ctypes.POINTER(ctypes.c_double)()
y_array_ptr = ctypes.POINTER(ctypes.c_double)()
size = ctypes.c_int()

# Call the C function
lib.runge_kutta(
    ctypes.c_double(x0),
    ctypes.c_double(y0),
    ctypes.c_double(v0),
    ctypes.c_double(x_end),
    ctypes.c_double(h),
    ctypes.byref(x_array_ptr),
    ctypes.byref(y_array_ptr),
    ctypes.byref(size),
)

# Extract the results into Python arrays
n = size.value
x_array = np.ctypeslib.as_array(x_array_ptr, shape=(n,))
y_array = np.ctypeslib.as_array(y_array_ptr, shape=(n,))

# Plot the results
plt.plot(x_array, y_array, label='sim')
plt.xlabel('x')
plt.ylabel('y')
plt.grid()
plt.legend()
plt.show()

# Free the allocated memory in C
lib.free(x_array_ptr)
lib.free(y_array_ptr)
