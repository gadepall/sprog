import matplotlib.pyplot as plt
from ctypes import CDLL, c_float, POINTER

# Load the shared library
newvalues = CDLL('./b.so')

# Declare the functions from the shared library
x_n = newvalues.x_n
y_1 = newvalues.y_1
y_n = newvalues.y_n
k1 = newvalues.k1
k2 = newvalues.k2
k3 = newvalues.k3
k4 = newvalues.k4

# Set return types for the C functions
x_n.restype = c_float
y_1.restype = c_float
y_n.restype = c_float

# Set argument types for RK4 stages
k1.argtypes = [c_float, c_float, c_float, c_float, POINTER(c_float), POINTER(c_float)]
k2.argtypes = [c_float, c_float, c_float, c_float, c_float, c_float, POINTER(c_float), POINTER(c_float)]
k3.argtypes = [c_float, c_float, c_float, c_float, c_float, c_float, POINTER(c_float), POINTER(c_float)]
k4.argtypes = [c_float, c_float, c_float, c_float, c_float, c_float, POINTER(c_float), POINTER(c_float)]

# Initial conditions
x_0 = 0.0
y_0 = 0.0
dy_0 = 1.0  # Assumed first derivative
h = 0.1  # Step size

# Finite Difference Method
x_values_fd = [x_0]
y_values_fd = [y_0]

# Computing x1
x1 = round(x_n(c_float(x_0), c_float(h)), 3)
x_values_fd.append(x1)

# Compute y1 using the first derivative
y1 = round(y_1(c_float(y_0), c_float(dy_0), c_float(h)), 3)
y_values_fd.append(y1)

# Compute subsequent y values using the finite difference method
for i in range(2, 100):
    x_i = round(x_n(c_float(x_values_fd[-1]), c_float(h)), 3)  # Update x
    y_i = round(y_n(c_float(y_values_fd[-1]), c_float(y_values_fd[-2]), c_float(h), c_float(x_values_fd[-1])), 3)
    x_values_fd.append(x_i)
    y_values_fd.append(y_i)

# RK4 Method
x_values_rk4 = [x_0]
y_values_rk4 = [y_0]
z_0 = dy_0

for i in range(1, 100):
    x = x_values_rk4[i - 1]
    y = y_values_rk4[i - 1]
    z = z_0

    # Compute RK4 stages
    k1_y, k1_z = c_float(), c_float()
    k1(c_float(h), c_float(x), c_float(y), c_float(z), POINTER(c_float)(k1_y), POINTER(c_float)(k1_z))

    k2_y, k2_z = c_float(), c_float()
    k2(c_float(h), c_float(x), c_float(y), c_float(z), k1_y, k1_z, POINTER(c_float)(k2_y), POINTER(c_float)(k2_z))

    k3_y, k3_z = c_float(), c_float()
    k3(c_float(h), c_float(x), c_float(y), c_float(z), k2_y, k2_z, POINTER(c_float)(k3_y), POINTER(c_float)(k3_z))

    k4_y, k4_z = c_float(), c_float()
    k4(c_float(h), c_float(x), c_float(y), c_float(z), k3_y, k3_z, POINTER(c_float)(k4_y), POINTER(c_float)(k4_z))

    # Update y and z
    y_new = y + (k1_y.value + 2 * k2_y.value + 2 * k3_y.value + k4_y.value) / 6
    z_new = z + (k1_z.value + 2 * k2_z.value + 2 * k3_z.value + k4_z.value) / 6
    x_new = x + h

    x_values_rk4.append(x_new)
    y_values_rk4.append(y_new)
    z_0 = z_new

# Plot the results
plt.plot(x_values_fd, y_values_fd, color='blue', label='sim1')
plt.plot(x_values_rk4, y_values_rk4, color='red', linestyle='--', label='sim2')
plt.xlabel("x")
plt.ylabel("y(x)")
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")

