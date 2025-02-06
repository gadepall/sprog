import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return 1/x - 1/(x-2) - 3

x = np.linspace(0.1, 4, 1000)
y = f(x)

plt.figure(figsize=(10, 6))
plt.plot(x, y, label='f(x) = 1/x - 1/(x-2) - 3')
plt.axhline(y=0, color='r', linestyle='--')
plt.axvline(x=0, color='k')
plt.axvline(x=2, color='k', linestyle='--')

roots = [1 + np.sqrt(3)/3, 1 - np.sqrt(3)/3]
for root in roots:
    plt.plot(root, 0, 'ro', markersize=10)
    plt.annotate(f'Root: {root:.4f}', (root, 0.1))

plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.grid(True)
plt.ylim(-10, 10)

# Save the figure
plt.savefig('fig.png', dpi=300, bbox_inches='tight')

# Display the plot
plt.show()

