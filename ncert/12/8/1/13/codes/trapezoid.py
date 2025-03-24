import ctypes
lib = ctypes.CDLL('./trapezoid.so')
lib.Area.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.Area.restype = ctypes.c_double

ll = 0.0 # Lower limit
ul = 3.0 # Upper limit
n = 1000 # Number of subintervals

print("Area bounded is: ",lib.Area(ll, ul, n))
