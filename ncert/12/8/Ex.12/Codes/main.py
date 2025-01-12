import numpy as np
import matplotlib.pyplot as plt
from ctypes import CDLL, Structure, POINTER, c_double, byref

# Load the shared library
lib = CDLL("./main.so")

# Specify the argument and return types for the C function
lib.Area.argtypes = [c_double, c_double]
lib.Area.restype = c_double

# Define initial conditions
x2 = -1  # value of x2
x1 = 1   # value for x1


plt.figure(figsize=(8, 6))

#Theoritical Plot
X = np.linspace(-2, 2, 5000)
Y = 3*X+2
plt.plot(X, Y, label="$y=3x+2$", color='red') #plotting y=3x+2
plt.axvline(x=1, color='blue', linestyle=':', linewidth=2, label='$x = 1$') #plotting x=1
plt.axvline(x=-1, color='green', linestyle=':', linewidth=2, label='$x = -1$') #plotting x=-1
plt.axhline(y=0, color='black', linestyle='--', linewidth=2, label='y = 0')#ploting x-axis
# Shade the region between x=-1 and x=1 
x_region=np.linspace(x2,x1,3000)
y_region=3*(x_region)+2
plt.fill_between(x_region, y_region, color='orange', alpha=0.3, label='Bounded Region')

# Call integrate
Area = lib.Area(c_double(x1), c_double(x2))
print("Area is",Area)

plt.xlabel("x")
plt.ylabel("y")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.savefig("figs/fig.png")
plt.show()
