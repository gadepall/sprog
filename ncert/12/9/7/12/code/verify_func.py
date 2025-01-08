import ctypes
import numpy as np
import matplotlib.pyplot as plt

lib = ctypes.CDLL('./source_func.so')
lib.finite_difference_method.argtypes = [
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_int,
    np.ctypeslib.ndpointer(dtype=np.float64),
    np.ctypeslib.ndpointer(dtype=np.float64)
]

def theoritical_solution(x):
    return np.exp(-2 * np.sqrt(x)) * (2 * np.sqrt(x) - 2)

x0, y0 = 1.0, 0.0
h = 0.01
n = 1000

x_values = np.zeros(n + 1)
y_values = np.zeros(n + 1)

lib.finite_difference_method(x0, y0, h, n, x_values, y_values)
y_analytical = theoritical_solution(x_values)

plt.figure(figsize=(10, 6))
plt.plot(x_values, y_values, 'r-', label='Sim')
plt.plot(x_values, y_analytical, 'b--', label='Theory')
plt.xlabel('x')
plt.ylabel('y')
#plt.title('Finite Difference vs Analytical Solution')
plt.legend()
plt.grid(True)
plt.savefig('../figs/curve.pdf', dpi=300)
plt.show()
