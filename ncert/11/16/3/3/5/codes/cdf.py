import numpy as np
import matplotlib.pyplot as plt

def cdf(x):
    if x < 1:
        return 0
    elif 1 <= x < 2:
        return 1/6
    elif 2 <= x < 3:
        return 2/6
    elif 3 <= x < 4:
        return 3/6
    elif 4 <= x < 5:
        return 4/6
    elif 5 <= x < 6:
        return 5/6
    else:
        return 1
x_values = np.array([0,1,2,3,4,5,6,7,8])
y_values = np.array([cdf(np.floor(x)) for x in x_values])
plt.figure(figsize=(8, 6))
markerline, stemlines, baseline = plt.stem(x_values, y_values)
plt.setp(markerline, 'markerfacecolor', 'red')
plt.setp(stemlines, 'color', 'red')
plt.setp(baseline, 'color', 'gray', 'linewidth', 1)

plt.xlabel('x')
plt.ylabel('$P(X)$')
plt.grid(True)
plt.savefig("../figs/cdf.png")
