import numpy as np
import matplotlib.pyplot as plt


def line1(x1):
    return (3900 - 3 * x1) / 6

def line2(x1):
    return (1300 - x1) / 3


x1 = np.linspace(0, 1500, 400)

# Compute y values for both lines
y1 = line1(x1)
y2 = line2(x1)

# Plot the lines
plt.figure(figsize=(8, 6))
plt.plot(x1, y1, label='3x1 + 6x2 = 3900', color='blue')
plt.plot(x1, y2, label='x1 + 3x2 = 1300', color='orange')
plt.scatter(1300, 0)
plt.annotate(f"Solution: (1300, 0)", 
             xy=(1300, 0), 
             xytext=(5, 10), 
             arrowprops=dict(arrowstyle='->', color='red'), 
             fontsize=10)
# Labels and legend
plt.xlabel('x1 (Price of a bat)')
plt.ylabel('x2 (Price of a ball)')
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
plt.legend()

plt.show()

