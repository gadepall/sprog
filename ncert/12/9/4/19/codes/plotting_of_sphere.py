import matplotlib.pyplot as plt
import numpy as np

radii = [3, 6]  #radii of growing spheres
time_points = [0, 3]  #Corresponding time points
fig, ax = plt.subplots(subplot_kw={"projection": "3d"}, figsize=(10, 6))

for i, r in enumerate(radii):
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    x = r * np.outer(np.cos(u), np.sin(v))
    y = r * np.outer(np.sin(u), np.sin(v))
    z = r * np.outer(np.ones(np.size(u)), np.cos(v))
    ax.plot_surface(x, y, z, alpha=0.3, color="b")
    ax.text(0, 0, r + 1, f"t = {time_points[i]}s", color="red", ha="center")


ax.set_xlim([-6, 6])
ax.set_ylim([-6, 6])
ax.set_zlim([-6, 6])

ax.set_xlabel("X-axis")
ax.set_ylabel("Y-axis")
ax.set_zlabel("Z-axis")

plt.savefig("growing_sphere.png")
plt.show()

