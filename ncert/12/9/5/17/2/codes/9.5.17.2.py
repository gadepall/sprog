import ctypes
import matplotlib.pyplot as plt
import numpy as np

# lib stores the shared object file of the c code
lib = ctypes.CDLL('./9.5.17.2.so')

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




# plotting both the graphs
plt.plot(xcoords,ycoords,label="sim",color='r',linewidth = 3) # plotting simulated graph

#labelling the two perpendicular axes
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
# To get a grid
plt.grid(True)
plt.show()



