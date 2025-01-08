import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object (.so) file
lib = ctypes.CDLL('./libintersection.so')

# Define the function signatures from the shared library
lib.calculate_intersection_and_area.argtypes = [
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int
]
lib.calculate_intersection_and_area.restype = ctypes.c_int

# Function to calculate the intersection points and area
def calculate_intersection_and_area(a, b, m, c):
    x1 = ctypes.c_double(0)
    y1 = ctypes.c_double(0)
    x2 = ctypes.c_double(0)
    y2 = ctypes.c_double(0)
    area_result = ctypes.c_double(0)
    
    # Call the C function
    num_intersections = lib.calculate_intersection_and_area(
        ctypes.byref(x1), ctypes.byref(y1),
        ctypes.byref(x2), ctypes.byref(y2),
        ctypes.byref(area_result),
        a, b, m, c, 2
    )
    
    return num_intersections, (x1.value, y1.value), (x2.value, y2.value), area_result.value

# Function to plot the graphs and shade the area
def plot_graph():
    # Define the line equation y = x and y = x^2
    x_vals = np.linspace(-0.5, 1.5, 400)
    y1_vals = x_vals  # y = x
    y2_vals = x_vals ** 2  # y = x^2
    
    # Plot the curves
    plt.plot(x_vals, y1_vals, label="y = x")
    plt.plot(x_vals, y2_vals, label="y = x^2")
    
    # Calculate the intersection points and area
    num_intersections, intersection1, intersection2, area = calculate_intersection_and_area(0, 0, 1, 0)
    
    # Display intersection points and area
    if num_intersections > 0:
        # Plot the intersection points on the graph
        plt.scatter([intersection1[0], intersection2[0]], [intersection1[1], intersection2[1]], color='red', zorder=5, label="Intersections")
        print(f"Intersection Points: {intersection1} and {intersection2}")
        print(f"Area between the curves: {area:.5f}")
        
        # Shade the area between the curves
        # Ensure the area is shaded only between the intersection points
        x_fill = np.linspace(intersection1[0], intersection2[0], 400)
        y1_fill = x_fill  # y = x
        y2_fill = x_fill ** 2  # y = x^2
        plt.fill_between(x_fill, y1_fill, y2_fill, color='gray', alpha=0.5, label="Shaded Area")
    else:
        print("No intersection points found.")
    
    # Customize the plot
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.grid(True)
    plt.savefig('fig.pdf')
    plt.show()

# Call the function to plot the graph
plot_graph()

