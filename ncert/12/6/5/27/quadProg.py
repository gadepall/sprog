import numpy as np
from cvxpy import *


#Parabola parameters
P = np.array([0,-5]).reshape(2,-1)
V = np.array([[1,0],[0,0]])
u = np.array([0,2]).reshape(2,-1)

x = Variable((2,1))

#Cost function
f =  quad_form(x-P, np.eye(2))
obj = Minimize(f)

#Constraints
constraints = [quad_form(x,V) + u.T@x <= 0]

#solution
Problem(obj, constraints).solve()

print(np.sqrt(f.value),x.value)
