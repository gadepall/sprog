import ctypes
import numpy as np
import matplotlib.pyplot as plt
from ctypes import c_double, c_int, POINTER, Structure

class Points(Structure):
    _fields_ = [
        ("x", POINTER(c_double)),
        ("y", POINTER(c_double)),
        ("size", c_int)
    ]

lib = ctypes.CDLL('./source_func.so')

lib.generate_points.argtypes = [c_double, c_double, c_double, c_int]
lib.generate_points.restype = Points
lib.free_points.argtypes = [Points]

def theoretical_curve(x):
    return -np.log(2 - np.exp(x))

x0 = -2.0 
y0 = -np.log(2 - np.exp(x0))
h = 0.01
steps = 1000
points = lib.generate_points(x0, y0, h, steps)
x_sim = np.array([points.x[i] for i in range(points.size)])
y_sim = np.array([points.y[i] for i in range(points.size)])

x_theo = np.linspace(-2, 0.693, 1000)  # Up to ln(2) ≈ 0.693 
y_theo = theoretical_curve(x_theo)
#WHEN WE USE x value as np.log(2) as geting a runtime error as y was close to infinity.
plt.figure(figsize=(10, 8))
plt.plot(x_theo, y_theo, 'r-', label='Theory')
plt.plot(x_sim, y_sim, 'b--', label='Sim')
plt.grid(True)
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.axis([-2, 1, -1, 6])

# Save the plot
plt.savefig('../figs/curve.pdf', dpi=300)
plt.show()
lib.free_points(points)

