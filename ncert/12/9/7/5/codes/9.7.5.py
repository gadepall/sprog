import ctypes
import matplotlib.pyplot as plt
import numpy as np

# lib stores the shared object file of the c code
lib = ctypes.CDLL('./9.7.5.so')

#the functions 'coords1' and 'coords2' in the C file is accessed via lib.coords 
lib.coords1.restype = ctypes.POINTER(ctypes.c_double) # this line of code specifies that the function returns a pointer to an array
lib.coords2.restype = ctypes.POINTER(ctypes.c_double)

result1 = lib.coords1() # storing the pointer returned by the function 'coords1' in "result1" 
result2 = lib.coords2() # storing the pointer returned by the function 'coords2' in "result2"

#initializing empty lists to store x coordinates and y coordinates of the generated points respectively
x1coords = list()
y1coords = list()
y11coords = list()

x2coords = list()
y2coords = list()
y22coords = list()


# even indices contain x coordinates and odd indices contain y coordinates , therefore, storing the coordinates in lists accordingly.
for i in range(0,205,2):
        x1coords.append(result1[i])
        y1coords.append(result1[i+1])
        y11coords.append(-result1[i+1] + 2) 
for i in range(0,201,2):
        x2coords.append(result2[i])
        y2coords.append(result2[i+1])
        y22coords.append(-result2[i+1] + 2)
        
        #Generating  x and y coordinates for the theoretical plot

theta = np.linspace(0, 2 * np.pi, 500)  # Parameter for the circle
x = 1 + np.cos(theta)  # x-coordinates of the circle
y = 1 + np.sin(theta)  # y-coordinates of the circle

#plotting theoretical
plt.plot(x, y, label="theory",color='r',linewidth =10)
# plotting simulated graph

plt.plot(x1coords,y1coords,marker='o',label="sim",color='k',linewidth=2) 
plt.plot(x1coords,y11coords,marker='o',color='k',linewidth=2)
plt.plot(x2coords,y2coords,marker='o',color='k',linewidth=2)
plt.plot(0,1,marker='o',color='k',linewidth=2)
plt.plot(1,1,marker='o',color='k',linewidth=2)
plt.text(1,1,f'(1,1)',fontsize=10,ha='right',va='bottom')
plt.plot(x2coords,y22coords,marker='o',color='k',linewidth=1)


#labelling the two perpendicular axeplt.axis('equal')
plt.xlim(0,2.2)
plt.ylim(0,2.2)
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
# To get a grid
plt.grid(True)
plt.show()




