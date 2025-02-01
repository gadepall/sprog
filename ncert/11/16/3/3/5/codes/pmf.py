import numpy as np
import matplotlib.pyplot as plt

def pmf(x):
    if x == 0:
        return 0
    elif x == 1:
        return 1/6
    elif x == 2:
        return 1/6
    elif x == 3:
        return 1/6
    elif x == 4:
        return 1/6
    elif x == 5:
        return 1/6
    elif x == 6:
        return 1/6
    else:
        return 0

x_values = np.array([0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8])
y_values = np.array([pmf(x) for x in x_values])

plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_values, y_values)
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

plt.xlabel('x')
plt.ylabel('$F_x(X)$')
plt.grid(True)
plt.savefig("../figs/pmf.png")

