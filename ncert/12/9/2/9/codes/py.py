import ctypes
import numpy as np
import matplotlib.pyplot as plt

lib = ctypes.CDLL('./plot_functions.so')

lib.finite_differences.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                                   ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int)]
lib.exact_solution.argtypes = [ctypes.c_double]
lib.exact_solution.restype = ctypes.c_double


y_start, y_end, h = 0.0, 5, 0.1
n_points = ctypes.c_int()

y_values = np.zeros(int((y_end - y_start) / h) + 1, dtype=np.float64)
x_values = np.zeros_like(y_values, dtype=np.float64)

lib.finite_differences(y_start, y_end, h,
                       y_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                       x_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
                       ctypes.byref(n_points))

plt.scatter(x_values[:n_points.value], y_values[:n_points.value], color='blue', label='sim',linestyle='dotted', s=20)

y_exact = np.linspace(y_start, y_end, 1000)
x_exact = [lib.exact_solution(y) for y in y_exact]
plt.plot(x_exact, y_exact, color='red', label='Theory')

plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()
plt.show()
