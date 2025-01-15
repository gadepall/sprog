import numpy as np
import ctypes
import matplotlib.pyplot as plt
import cvxpy as cp

# Define variables
x = cp.Variable(pos=True)  # x > 0
y = cp.Variable(pos=True)  # y > 0

# Define the constraints
constraints = [x + y == 60]  # Equality constraint

# Define the objective function
objective = cp.Maximize(cp.log(x) + 3 * cp.log(y))  # Maximize log(x) + 3*log(y)

# Formulate the problem
problem = cp.Problem(objective, constraints)

# Solve the problem
problem.solve()

# Extract results
optimal_x = x.value
optimal_y = y.value
optimal_f = optimal_x * (optimal_y ** 3)  # f(x) = x * y^3

# lib stores the shared object file of the c code
lib = ctypes.CDLL('./6.5.14.so')

#the function 'gradientascent' is accessed via lib.gradientascent
#the restype(type of output) by the function gradientascent.
lib.gradientascent.restype = ctypes.POINTER(ctypes.c_double)


result = lib.gradientascent() # storing the pointer returned by the function 'gradientascent' in result

#initializing  lists to store x coordinates and y coordinates of the generated points respectively.
xcoords = np.linspace(13,17,400)
ycoords = xcoords*((60-xcoords)**3)
#storing x ang y coords of maxima.       
xmax = result[0]
ymax = result[1]

print("x value at maxima = ",xmax)
print("y value at maxima = ",60 - xmax)

#plotting theoretical curve
plt.plot(xcoords, ycoords,color='r',linewidth =2,label="functional curve")
#plot the maxima
plt.scatter(xmax,ymax,s = 400,color='k',label="sim1 maxima")

y1 = (optimal_x)*((optimal_y)**3)
y2 = 15*45*45*45
plt.scatter(optimal_x,y1,s = 200,color='g',label="sim2 maxima")
plt.scatter(15,y2,color='m',s=100,label = "theory maxima")
print("value for x using geometric programming" ,optimal_x)
print("value for y using geometric programming",optimal_y)
#labelling the two perpendicular axes
plt.xlim(13,17)
plt.ylim(1349699,1367500)
plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
# To get a grid
plt.grid(True)
plt.show()



