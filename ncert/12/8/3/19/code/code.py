import numpy as np
import matplotlib.pyplot as plt

def func(x):
    sol = np.zeros(len(x))
    for i in range(len(x)):
        sol[i] = np.min([np.sin(x[i]), np.cos(x[i])])
    return sol
    # return(np.min([np.sin(x), np.cos(x)]))

def func_(x):
    return np.min([np.sin(x), np.cos(x)])

interations = 1000
area = 0

del_x = np.pi/2 / interations

# for i in range(1, interations-1):
#     area += del_x * func()


x = np.linspace(0, np.pi /2, interations)
x1 = np.linspace(-1, 3, 1000)
sin = np.sin(x1)
cos = np.cos(x1)

sum_ = np.sum(func(x))
area = del_x * (sum_ - (func_(x[0]) + func_(x[interations-1]))/2)

plt.plot(x1, sin, label = "$sin(x)$")
plt.plot(x1, cos, label = "$cos(x)$")
plt.plot(x, func(x), label = 'curve')

cond_sin = np.zeros(len(x))
cond_cos = np.zeros(len(x))
for i in range(len(x1)):
    if x1[i]>0 and x1[i]<np.pi/4: cond_sin[i] = True
    else: cond_sin[i] = False

for i in range(len(x)):
    if x1[i]>np.pi/4 and x1[i] < np.pi/2: cond_cos[i] = True
    else: cond_cos[i] = False

plt.fill_between(x1, sin, 0, where=cond_sin, color = 'red', alpha = 0.5, label = f'Area = {area/2}')
plt.fill_between(x1,cos,0, where=cond_cos, color = 'blue', alpha = 0.5, label = f'Area = {area/2}')
ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

plt.axis('equal')
plt.grid()
plt.legend(loc='best')

plt.savefig("../figs/fig1.png")
plt.show()
