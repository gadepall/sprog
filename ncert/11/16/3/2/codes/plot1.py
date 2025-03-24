import numpy as np
import matplotlib.pyplot as plt
import ctypes

gen = ctypes.CDLL('./sim.so')
gen.theoretical_binomial.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.POINTER(ctypes.c_double)]
gen.theoretical_binomial.restype = None
gen.sim_binomial.argtypes = [ctypes.c_int, ctypes.c_double, ctypes.POINTER(ctypes.c_double)]
gen.sim_binomial.restype = None
n = 2
p = 0.5
x_values = np.array([x for x in range(n + 1)])
y_values = np.zeros((1, n+1), dtype = np.double)
gen.theoretical_binomial(
        n, p, y_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
        )
print(x_values)
print(y_values)
# Create the stem plot
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_values, y_values[0], label = "theory")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

gen.sim_binomial(
        n, p, y_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
        )
markerline, stemlines, baseline = plt.stem(x_values, y_values[0], label = "sim")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'blue')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)
plt.legend(loc = "best")
plt.xlabel('x')
plt.xlim(-0.5, 2.5)
plt.ylabel('f(x)')
plt.grid(True)
plt.savefig("../figs/fig1.png")

