import numpy as np
import matplotlib.pyplot as plt

func = lambda x: x**2 -60*x - 2700

# g_x = lambda x: (x**2 - 2700) / 60

#Fixed-Point Interations

g_x = lambda x: 2700/x + 60

x_range = np.linspace(0, 120, 1000)

x_0 = 150
x_1 = g_x(x_0)
c = 0
while np.abs(x_1 - x_0) > 10e-10:
    x_0 = x_1
    x_1 = g_x(x_0)
    print(x_0)
    c+=1
print(c)

#Newton's iteration
print('')
c=0
diff = lambda x: 2 * x - 60
interation = lambda x: x - func(x)/diff(x)

x_0 = 150
x_1 = interation(x_0)

while(np.abs(x_1 - x_0)>10e-10):
    x_0 = x_1
    x_1 = interation(x_0)
    print(x_0)
    c+=1
print(c)

#secant method
x_0 = 150
x_1 = 50

iteration = lambda x1, x2: x2 - (func(x2)*(x2-x1))/(func(x2) - func(x1))

x_2 = iteration(x_0, x_1)
c = 0
while np.abs(x_2 - x_1) > 10e-10:
    x_0 = x_1
    x_1 = x_2
    x_2 = iteration(x_0, x_1)
    print(x_2)
    c+=1
print(c)

M = np.array([[0, 2700],[1, 60]])

lam = np.linalg.eig(M)
print(lam)

plt.plot(x_range, func(x_range), label = 'function')
plt.scatter(x_2, func(x_2), color = 'red', label = 'solution')


ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

# plt.axis('equal')
plt.grid()
plt.legend(loc='best')

plt.savefig("../figs/fig1.png")
plt.show()