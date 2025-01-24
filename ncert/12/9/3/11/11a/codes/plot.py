import numpy as np
import matplotlib.pyplot as plt

# Load data from the file
data = np.loadtxt("output_data.txt", delimiter=",", skiprows=1)

x = data[:, 0]           # x values
y_numerical = data[:, 1] # Numerical solution
y_cos_sin = data[:, 2]   # Exact (cos + sin)
y_exp = data[:, 3]       # Exact (exp form)

# Plot the results
plt.plot(x, y_numerical, label="Numerical", color="blue")
plt.plot(x, y_cos_sin, label="Exact (cos + sin)", linestyle="--", color="green")
plt.plot(x, y_exp, label="Exact (exp)", linestyle=":", color="red")

plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid()
plt.savefig("comparison_plot.png", dpi=300, bbox_inches='tight')
plt.show()



