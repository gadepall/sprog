import numpy as np
import matplotlib.pyplot as plt
import ctypes


# Load the shared library
lib = ctypes.CDLL('./lib.so')


lib.function.argtypes=[ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
lib.f.argtypes=[ctypes.c_double]
lib.f.restype=ctypes.c_double
lib.f_prime.argtypes=[ctypes.c_double]
lib.f_prime.restype=ctypes.c_double
lib.NR.argtypes = [ctypes.c_double]
lib.NR.restype = ctypes.c_double

# Parameters
x_start = 0
x_end = 40
h = 0.1
n_steps = 401

x = np.linspace(x_start, x_end, n_steps)
y = np.zeros(n_steps)


# Conversion to ctypes array
x_ctypes = x.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_ctypes = y.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

lib.function(x_ctypes, y_ctypes, n_steps)
root1=lib.NR(5)
root2=lib.NR(25)


# Plotting
plt.figure(figsize=(10, 6))

plt.plot(x, y, label="Theory", linestyle='-', color='b', linewidth=5)
plt.scatter(root1, 0, color='r', marker='o', s=100, zorder=5, label=f"Root at x={root1:.2f}")
plt.scatter(root2, 0, color='r', marker='o', s=100, zorder=5, label=f"Root at x={root2:.2f}")
plt.xlabel("x")
plt.ylabel("y")
plt.legend(['f(x)=$x^2-45x+324$',f"Root at x={root2:.2f}",f"Root at x={root1:.2f}"])
plt.grid()
plt.savefig('plot.png')



