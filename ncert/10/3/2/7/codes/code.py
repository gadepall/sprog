import numpy as np
import matplotlib.pyplot as plt

# Define the coefficients of the equations
# Line 1: x - y + 1 = 0  => x - y = -1
# Line 2: 3x + 2y - 12 = 0 => 3x + 2y = 12
A = np.array([[1, -1], [3, 2]])
B = np.array([-1, 12])

# Perform LU decomposition manually
def lu_decomposition(a):
    n = a.shape[0]
    L = np.zeros((n, n))
    U = np.zeros((n, n))

    for i in range(n):
        # Compute U matrix
        for k in range(i, n):
            U[i][k] = a[i][k] - sum(L[i][j] * U[j][k] for j in range(i))

        # Compute L matrix
        for k in range(i, n):
            if i == k:
                L[i][i] = 1
            else:
                L[k][i] = (a[k][i] - sum(L[k][j] * U[j][i] for j in range(i))) / U[i][i]

    return L, U

L, U = lu_decomposition(A)

# Forward substitution for Lc = B
n = L.shape[0]
c = np.zeros(n)
for i in range(n):
    c[i] = (B[i] - sum(L[i][j] * c[j] for j in range(i)))

# Backward substitution for Ux = c
x = np.zeros(n)
for i in range(n - 1, -1, -1):
    x[i] = (c[i] - sum(U[i][j] * x[j] for j in range(i + 1, n))) / U[i][i]

intersection = x

# Find x- and y-intercepts of both lines
# Line 1: x - y = -1
x1_intercept = -1  # Set y = 0
y1_intercept = 1   # Set x = 0

# Line 2: 3x + 2y = 12
x2_intercept = 4   # Set y = 0
y2_intercept = 6   # Set x = 0

# Points forming the triangle
triangle_points = np.array([
    [x1_intercept, 0],
    [x2_intercept, 0],
    intersection
])

# Plot the lines
x = np.linspace(-2, 6, 500)
line1 = x + 1   # y = x + 1
line2 = (12 - 3 * x) / 2  # y = (12 - 3x) / 2

plt.figure(figsize=(8, 8))
plt.plot(x, line1, label="x - y + 1 = 0")
plt.plot(x, line2, label="3x + 2y - 12 = 0")

# Highlight the triangle
plt.fill(triangle_points[:, 0], triangle_points[:, 1], color='lightblue', alpha=0.5, label='Triangle')

# Annotate points
plt.scatter(triangle_points[:, 0], triangle_points[:, 1], color='red', zorder=5)
for i, point in enumerate(triangle_points):
    plt.text(point[0] + 0.2, point[1] + 0.2, f"P{i + 1}({point[0]:.2f}, {point[1]:.2f})")

# Plot settings
plt.axhline(0, color='black',linewidth=0.5)
plt.axvline(0, color='black',linewidth=0.5)
plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
plt.legend()
plt.title("Graph of Lines and Triangle Formed")
plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.show()

# Print the coordinates of the vertices
print("Coordinates of the triangle vertices:")
print(f"P1: ({x1_intercept}, 0)")
print(f"P2: ({x2_intercept}, 0)")
print(f"P3 (Intersection Point): ({intersection[0]:.2f}, {intersection[1]:.2f})")

