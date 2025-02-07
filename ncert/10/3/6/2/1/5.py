#Python code for plotting
import numpy as np
import matplotlib.pyplot as plt

#Line parameters
A = np.array([[1, 1], [1, -1]])
b = np.array([10, 2])

#Solving equations
x = np.linalg.solve(A,b)

#Plotting points
v = np.linspace(0,10,100)

#Generating points for plotting
c1 = 10 - v  #v + c = 10 (downstream)
c2 = v - 2   #v - c = 2 (upstream)

#Create the plot
plt.figure(figsize=(8, 6))
plt.plot(v, c1, label='v + c = 10 (downstream)')
plt.plot(v, c2, label='v - c = 2 (upstream)')

#Plot the intersection point
plt.plot(6, 4, 'ro', label='Solution (6,4)')

#Add labels 
plt.xlabel('v (speed in still water)')
plt.ylabel('c (speed of current)')
plt.grid(True)
plt.legend()

#Set axis limits
plt.xlim(0, 10)
plt.ylim(0, 10)

#Save the plot
plt.savefig('figs/fig.png', bbox_inches='tight')
plt.show()

