import ctypes
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

# Load the shared library
lib = ctypes.CDLL('./libintegrator.so')

# Define the function signature of the C function `integrate`
lib.integrate.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
lib.integrate.restype = ctypes.c_double

# Function to calculate the integral using the shared library
def calculate_integral(a, b, n):
    return lib.integrate(a, b, n)

# Example functions to plot
def f1(x):
    return (3 * x + 12) / 2 - (3 * x ** 2) / 4

def f2(x):
    return (3 * x + 12) / 2  # Just another example function for comparison

# Plotting the functions and shading the area
def plot_functions(a, b, n):
    x_vals = np.linspace(a, b, 500)
    y_vals_f1 = f1(x_vals)
    y_vals_f2 = f2(x_vals)

    # Create the plot
    fig, ax = plt.subplots()

    # Plot the functions
    ax.plot(x_vals, y_vals_f1, label=r"$f_1(x) = \frac{3x + 12}{2} - \frac{3x^2}{4}$", color='blue')
    ax.plot(x_vals, y_vals_f2, label=r"$f_2(x) = \frac{3x + 12}{2}$", color='green')

    # Shade the region between the functions
    ix = np.linspace(a, b, 500)
    iy = f1(ix)
    ax.fill_between(ix, iy, f2(ix), color='gray', alpha=0.5, label='Shaded Area')

    # Add x and y axis lines
    ax.axhline(0, color='black',linewidth=1)  # x-axis
    ax.axvline(0, color='black',linewidth=1)  # y-axis

    # Add gridlines
    ax.grid(True)

    # Set up labels and title
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_title('Functions and Shaded Area Under the Curve')

    # Show the legend
    ax.legend()

    # Display the plot
    plt.show()

# Main function to integrate and plot
if __name__ == "__main__":
    # Set the integration limits and number of subintervals
    a = -2.0  # Start of the interval
    b = 4.0   # End of the interval
    n = 100    # Number of subintervals for the trapezoidal rule

    # Calculate the integral using the C library
    result = calculate_integral(a, b, n)
    print(f"Approximated area under the curve: {result:.5f}")

    # Plot the functions and shaded area
    plot_functions(a, b, n)

