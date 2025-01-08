import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Loading the shared library 
lib = ctypes.CDLL('./de1.so') 
# When we load a shared library using ctypes.CDLL, we get a Python object that corresponds to the C library. This object allows us to call the functions defined in the C library.
#lib is the variable in which we store this object.



# Define argument types for the C function (x and y arrays, and the length n)
lib.finite_difference.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.c_int]
#In Python, we inform ctypes of the expected types of the arguments when calling this function from Python using the argtypes attribute.
#ctypes.POINTER(ctypes.c_double) is used for passing arrays of doubles from Python to C. In C, arrays are passed as pointers to the first element of the array, and ctypes.POINTER allows Python to pass that reference correctly.


# Allocate memory for the x and y arrays
n = 2000000
 # Create an array of doubles with size n
x = (ctypes.c_double * n)() 
# Create an array of doubles with size n
y = (ctypes.c_double * n)()  

# Call the C function to populate the arrays
lib.finite_difference(x, y, n)

# Convert ctypes arrays to NumPy arrays for further processing
x_array = np.ctypeslib.as_array(x)
y_array = np.ctypeslib.as_array(y)

# Plot the points using matplotlib
plt.scatter(x_array, y_array, label='Simulation graph',color='yellow')
# Plotting the actual curve for verification

# Defining the range of x values and the function y = x^3 / 4
x = np.linspace(0, 2000, 50000)
y = x**3 / 4

# Plotting the curve
plt.plot(x, y, label=r"Theory curve", color="red")

# Adding a legend to distinguish this curve
plt.legend(fontsize=12)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Finite Difference Method: Numerical Solution')
plt.legend()
plt.show()

