import numpy as np
import matplotlib.pyplot as plt
import ctypes

gen = ctypes.CDLL('./sim.so')
gen.sim_cdf.argtpes = [ctypes.c_int, ctypes.c_double, ctypes.POINTER(ctypes.c_double)]
gen.sim_cdf.restype = None
gen.theoretical_cdf.argtpes = [ctypes.c_int, ctypes.c_double, ctypes.POINTER(ctypes.c_double)]
gen.theoretical_cdf.restype = None

n = 2 
p = 0.5

x_values = np.array([x for x in range(n+ 1)])
y_values = np.zeros((1, n+1), dtype=np.double)
x_values = np.append(x_values, -1)
gen.theoretical_cdf(
        n, ctypes.c_double(p), y_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
        )
y_values = np.append(y_values, 0)
print("Theoretical values of probability: ", y_values)
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_values, y_values, label = "theory")
plt.setp(markerline, 'markerfacecolor', 'green')
plt.setp(stemlines, 'color', 'green')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

y_values = np.zeros((1, n+1), dtype=np.double)
gen.sim_cdf(
        n, ctypes.c_double(p), y_values.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
        )
y_values = np.append(y_values, 0)
print("Simulated values of probability: ", y_values)
markerline, stemlines, baseline = plt.stem(x_values, y_values, label = "sim")
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

plt.xlabel('x')
plt.ylabel('f(x)')
plt.grid(True)
plt.legend(loc='best')
plt.legend()
plt.savefig("../figs/fig2.png")
