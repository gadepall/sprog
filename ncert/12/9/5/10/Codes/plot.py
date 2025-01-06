import numpy as np
import ctypes
import matplotlib.pyplot as plt
lib=ctypes.CDLL('./libfunc.so') #shared library
lib.calculate_points_1.argtypes=[ctypes.c_float, ctypes.c_float, ctypes.c_int, 
                                 ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_double]
lib.calculate_points_1.restype = None #defining the argument types
x0=-1
y0=0 # giving the values of x_0 and y_0 as calculated in theory
h=0.01 # h has to tend to zero, but approximating
n=100 # no. of values to calculate
x1=np.zeros(n,dtype=np.float32) #making numpy arrays for x and y coordinates
y1=np.zeros(n,dtype=np.float32)
x_ctypes1 = x1.ctypes.data_as(ctypes.POINTER(ctypes.c_float)) #converting them to cpointers to use ctypes
y_ctypes1 = y1.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

lib.calculate_points_1(x0, y0, n, x_ctypes1, y_ctypes1, h) # calling the c function
x2=np.linspace(-1,0,50) #x coordinates to plot the theoretical plot
y2=-x2-1 #y coordinates to plot theoretical plot
plt.plot(x1,y1,label="sim-1",color='black',linewidth=4) #plotting the graph of simulated function
plt.plot(x2,y2,label="theory",color='lightgrey',linestyle='dotted',linewidth=5) #plotting theoretical function


#to plot using difference equation of dx/dy
lib.calculate_points_2.argtypes=[ctypes.c_float, ctypes.c_float, ctypes.c_int, 
                                 ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_double]
lib.calculate_points_2.restype = None #defining the argument types
x3=np.zeros(n,dtype=np.float32) #making numpy arrays for x and y coordinates
y3=np.zeros(n,dtype=np.float32)
x_ctypes2 = x3.ctypes.data_as(ctypes.POINTER(ctypes.c_float)) #converting them to cpointers to use ctypes
y_ctypes2 = y3.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
lib.calculate_points_1(x0, y0, n, x_ctypes2, y_ctypes2, h) # calling the c function
plt.plot(x3,y3,label="sim-2",color='red',linewidth=2,linestyle="dashdot") #plotting the graph of simulated function
#to plot z transform solved equation
lib.calculate_points_3.argtypes=[ctypes.c_float, ctypes.c_float, ctypes.c_int, 
                                 ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.c_double]
lib.calculate_points_3.restype = None #defining the argument types
x4=np.zeros(n,dtype=np.float32) #making numpy arrays for x and y coordinates
y4=np.zeros(n,dtype=np.float32)
x_ctypes3 = x4.ctypes.data_as(ctypes.POINTER(ctypes.c_float)) #converting them to cpointers to use ctypes
y_ctypes3 = y4.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
lib.calculate_points_3(x0, y0, n, x_ctypes3, y_ctypes3, 0.0001) # calling the c function
plt.plot(x4,y4,label="sim-3",color='blue',linewidth=3,linestyle="dotted") #plotting the graph of simulated function
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.grid()
plt.legend()
plt.show()

