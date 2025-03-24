import ctypes
import matplotlib.pyplot as plt
import ctypes
import numpy as np

# Load the shared library
lib = ctypes.CDLL("./func.so")

# Define the function signatures
# void trapezoidal(int n, double x[], double y[], double h, double x0, double y0);
lib.trapezoidal.argtypes = [
    ctypes.c_int,                               # n
    ctypes.POINTER(ctypes.c_double),            # x[]
    ctypes.POINTER(ctypes.c_double),            # y[]
    ctypes.c_double,                            # h
    ctypes.c_double,                            # x0
    ctypes.c_double                             # y0
]
lib.trapezoidal.restype = None  # void

# void bilinear(int n, double x[], double y[], double h, double x0, double y0);
lib.bilinear.argtypes = [
    ctypes.c_int,                               # n
    ctypes.POINTER(ctypes.c_double),            # x[]
    ctypes.POINTER(ctypes.c_double),            # y[]
    ctypes.c_double,                            # h
    ctypes.c_double,                            # x0
    ctypes.c_double                             # y0
]
lib.bilinear.restype = None  # void

# Parameters
n = 100               # Number of points
h = 0.01               # Step size
x0 = 0.0              # Initial x
y0 = 1.0              # Initial y

# Create arrays for results
x_array = np.zeros(n, dtype=np.float64)
y_array = np.zeros(n, dtype=np.float64)

# Convert numpy arrays to C pointers
x_ptr = x_array.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_ptr = y_array.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Call the trapezoidal function
lib.trapezoidal(n, x_ptr, y_ptr, h, x0, y0)
# Create arrays for results
x_2 = np.zeros(n, dtype=np.float64)
y_2 = np.zeros(n, dtype=np.float64)

# Convert numpy arrays to C pointers
x_ptr2 = x_2.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_ptr2 = y_2.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
lib.bilinear(n,x_ptr2,y_ptr2,h,x0,y0)
x3=np.linspace(0,1,100)
y3=(np.log(x3+1)/2) + (3*np.log(x3**2+1)/4) - (np.arctan(x3)/2) +1
plt.plot(x3,y3,label='theory')
plt.plot(x_2,y_2,label='sim-1',linestyle="--",linewidth="2")
plt.plot(x_array, y_array, label='sim-2',linestyle="dotted")
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.show()

