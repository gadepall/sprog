import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# Linking the compiled .so file to the Python script
dll = ctypes.CDLL('./ellipse.so')

# Setting the argument types and return types for the functions in the C code
dll.pointsGenerateLine.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.pointsGenerateLine.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.pointsGenerateEllipse.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.pointsGenerateEllipse.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.area.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_int]
dll.area.restype = ctypes.c_float

dll.freeMemory.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMemory.restype = None

# Ellipse parameters
a = 5
b = 3

# Line parameters
x0_line = 0
y0_line = b

# Integration parameters
x1 = 0
x2 = a * b / (a + b)  # Intersection point of the line and ellipse
n = 100000
h = (x2 - x1) / n

# Generating points on the line and ellipse
pointsLine = dll.pointsGenerateLine(a, b, x0_line, y0_line, h, n)
pointsEllipse = dll.pointsGenerateEllipse(a, b, x1, h, n)

# Calculating the area of the smaller region
area = dll.area(a, b, x1, x2, n)
print("The area of the smaller region is:\n", area)

# Storing the coordinates for plotting
coordinatesLine = []
coordinatesEllipse = []

for point in pointsLine[:n]:
    coordinatesLine.append(np.array([[point[0], point[1]]]).reshape(-1, 1))
coordinatesLine = np.block(coordinatesLine)

for point in pointsEllipse[:n]:
    coordinatesEllipse.append(np.array([[point[0], point[1]]]).reshape(-1, 1))
coordinatesEllipse = np.block(coordinatesEllipse)

# Plotting the results
plt.plot(coordinatesLine[0, :], coordinatesLine[1, :], label="Line: x/a + y/b = 1", color="red", linewidth=1.5)
plt.plot(coordinatesEllipse[0, :], coordinatesEllipse[1, :], label="Ellipse: x^2/a^2 + y^2/b^2 = 1", color="blue", linewidth=1.5)

# Fill the smaller region
plt.fill_between(
    coordinatesLine[0, :], coordinatesLine[1, :], coordinatesEllipse[1, :], color="green", alpha=0.2, label="Bounded Region"
)

plt.xlabel("x")
plt.ylabel("y")
plt.title("Ellipse and Line Intersection")
plt.legend()
plt.grid(True)
plt.show()

# Freeing the used memory
dll.freeMemory(pointsLine, n)
dll.freeMemory(pointsEllipse, n)

