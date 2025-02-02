import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Load the C library
try:
    clib = ctypes.CDLL("./gradient.so")  # Linux/macOS
except OSError:
    clib = ctypes.CDLL("./cone_volume.dll")  # Windows

# Define C function signatures
clib.computeVolume.argtypes = [ctypes.c_float, ctypes.c_float]
clib.computeVolume.restype = ctypes.c_float
clib.gradientAscent.argtypes = [
    ctypes.POINTER(ctypes.c_float), 
    ctypes.c_float, 
    ctypes.POINTER(ctypes.c_float), 
    ctypes.c_int, 
    ctypes.c_float,
    np.ctypeslib.ndpointer(dtype=np.float32),
    np.ctypeslib.ndpointer(dtype=np.float32),
    ctypes.POINTER(ctypes.c_int)
]
clib.gradientAscent.restype = ctypes.c_float
def compute_volume_python(h, r):  # Python version
    x = np.sqrt(2*r*h - h*h)
    return (1/3) * np.pi * x**2 * h


# Parameters
r = 1.0
alpha = 0.0001
h0 = 0.5
max_iter = 100000

# Prepare arrays for tracking
height_points = np.zeros(max_iter, dtype=np.float32)
volume_points = np.zeros(max_iter, dtype=np.float32)
point_count = ctypes.c_int(0)

# Prepare input arrays
h = np.array([h0], dtype=np.float32)
gradient = np.array([0.0], dtype=np.float32)

# Run gradient ascent
optimal_height = clib.gradientAscent(
    h.ctypes.data_as(ctypes.POINTER(ctypes.c_float)), 
    ctypes.c_float(alpha), 
    gradient.ctypes.data_as(ctypes.POINTER(ctypes.c_float)), 
    ctypes.c_int(max_iter), 
    ctypes.c_float(r),
    height_points,
    volume_points,
    ctypes.byref(point_count)
)

# Trim arrays to actual points
height_points = height_points[:point_count.value]
volume_points = volume_points[:point_count.value]

# Find maximum point
max_index = np.argmax(volume_points)
max_height = height_points[max_index]
max_volume = volume_points[max_index]
heights = np.linspace(0, 2 * r, 500)  # Up to the sphere's diameter
volumes_python = np.array([compute_volume_python(h, r) for h in heights])



# Plot
plt.figure(figsize=(10, 6))
plt.plot(heights, volumes_python, label="Actual graph")
plt.plot(height_points, volume_points,'--', label="gradient ascent")
plt.scatter(max_height, max_volume, color="red", label="Maximum Point")
plt.xlabel("Height (h)")
plt.ylabel("Volume (V)")
plt.title("Gradient Ascent")
plt.legend()
plt.grid(True)
plt.savefig('../figs/plot.pdf')
plt.show()

print(f"Optimal height: {max_height:.6f}")
print(f"Maximum volume: {max_volume:.6f}")
