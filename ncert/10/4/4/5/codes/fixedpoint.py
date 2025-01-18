import ctypes

# Load the shared library
lib = ctypes.CDLL('./fixedpoint.so')

# Define the argument and return types of the C function
lib.fixed_point_iteration.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.fixed_point_iteration.restype = ctypes.c_double

# Call the C function through ctypes
initial_guess = 10.0
tolerance = 1e-6
max_iter = 1000

result = lib.fixed_point_iteration(initial_guess, tolerance, max_iter)
print(f"The approximate root is: {result:.6f}")

