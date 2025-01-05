import ctypes
import matplotlib.pyplot as plt
import numpy as np

# lib stores the shared object file of the c code
lib = ctypes.CDLL('./9.5.13.so')

#the function 'coords' in the C file is accessed via lib.coords 
lib.coords.restype = ctypes.POINTER(ctypes.c_double) # this line of code specifies that the function returns a pointer to an array

result = lib.coords() # storing the pointer returned by the function in "result" 

#initializing two empty lists to store x coordinates and y coordinates of the generated points respectively
xcoords = list()
ycoords = list()

# even indices contain x coordinates and odd indices contain y coordinates , therefore, storing the coordinates in two lists accordingly.
for i in range(0,1999,2):
        xcoords.append(result[i])
        ycoords.append(result[i+1])

#plotting the theoretical(actual) graph of the function.
def function(x):
   return np.arctan(1/(np.log(x) + 1))*x # as cot^{-1}(y) = arctan(1/y)

#Generating y coordinates for the theoretical plot
y = function(xcoords)

# plotting both the graphs
plt.plot(xcoords,ycoords,label="sim",color='r',linewidth = 10) # plotting simulated graph
plt.plot(xcoords,y,label="theory",color='k',linestyle = "--" ,linewidth = 7) #  plotting theoretical graph

#labelling the two perpendicular axes
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
# To get a grid
plt.grid(True)
plt.show()




