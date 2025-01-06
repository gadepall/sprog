import numpy as np
import matplotlib.pyplot as plt


def dif(x, y):
    return np.sin(x) - 2*y

def theoretical_func(x):
    return (2 * np.sin(x) - np.cos(x) + np.exp(-2 * x))/5

resolution = 20
x1 = 0
x2 = 5

x = np.linspace(x1, x2, resolution)
h = (x2 - x1) / resolution

y = np.zeros(resolution)
y[0] = 0

for i in range(1, resolution):
    y[i] = y[i-1] + dif(x[i-1], y[i-1]) * h

plt.scatter(x, y, label = 'Plot by finite differences')

y_theoretical = theoretical_func(x)

plt.plot(x, y_theoretical, color = 'red', label = 'Actual Solution of the Differntial Equation')

ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

plt.axis('equal')
plt.grid()
plt.legend(loc='best')

plt.savefig('../../figs/fig1.png')
# plt.show()
