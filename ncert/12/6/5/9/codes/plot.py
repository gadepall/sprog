import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
gradient_lib = ctypes.CDLL('./gradient.so')

# Define C function prototypes
gradient_lib.func.restype = ctypes.c_double
gradient_lib.func.argtypes = [ctypes.c_double]

gradient_lib.func_deriv.restype = ctypes.c_double
gradient_lib.func_deriv.argtypes = [ctypes.c_double]

gradient_lib.gradient_method.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_double,
    ctypes.POINTER(ctypes.c_double),
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
]

gradient_lib.generate_points.argtypes = [
    ctypes.c_double, ctypes.c_double, ctypes.c_int,
    np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags="C_CONTIGUOUS"),
    np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags="C_CONTIGUOUS"),
    ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
]

# Wrappers for C functions
FUNC_TYPE = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
c_func = FUNC_TYPE(gradient_lib.func)
c_func_deriv = FUNC_TYPE(gradient_lib.func_deriv)

# Gradient ascent
x0 = 0.0  # Initial guess
learning_rate = 0.1
tol = 1e-6
extremum = (ctypes.c_double * 2)()
gradient_lib.gradient_method(x0, learning_rate, tol, extremum, c_func, c_func_deriv)

max_x = extremum[0]
max_f = extremum[1]

# Generate points for plotting
start, end, num_points = -2 * np.pi, 2 * np.pi, 500
x_vals = np.zeros(num_points)
y_vals = np.zeros(num_points)
gradient_lib.generate_points(start, end, num_points, x_vals, y_vals, c_func)

# Plot the function and the maximum point
plt.figure(figsize=(10, 6))
plt.plot(x_vals, y_vals, label="f(x) = sin(x) + cos(x)")
plt.scatter([max_x], [max_f], color="red", label=f"Maximum: ({max_x:.2f}, {max_f:.2f})")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
plt.axvline(max_x, color="red", linewidth=0.5, linestyle="--")
plt.legend()
plt.grid()
plt.show()

# Print the results
print(f"Maximum value of f(x): {max_f:.6f} at x = {max_x:.6f}")

