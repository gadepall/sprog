import numpy as np
import matplotlib.pyplot as plt
import ctypes

# Define the function f(x) = |sin(4x) + 3|
def f(x):
    return np.abs(np.sin(4 * x) + 3)

# Numerical approximation of the derivative of f(x)
def df(x, delta=1e-5):
    return (f(x + delta) - f(x - delta)) / (2 * delta)

# Define the shared library and functions
# Assuming the compiled shared library is named 'code.so'
c_lib = ctypes.CDLL('./code.so')  # Replace with the actual name of your .so file

# Define argument and return types for the C function 'find_extremum'
c_lib.find_extremum.argtypes = [
    ctypes.c_double,  # init_guess
    ctypes.c_double,  # step_size
    ctypes.c_double,  # tolerance
    ctypes.c_double,  # delta
    ctypes.c_int,     # is_max (1 for max, 0 for min)
    ctypes.POINTER(ctypes.c_double)  # extremum (output)
]

def find_extremum(init_guess, step_size, tolerance, delta=1e-5, is_max=0):
    extremum = ctypes.c_double(0)  # Placeholder for the result
    c_lib.find_extremum(init_guess, step_size, tolerance, delta, is_max, ctypes.byref(extremum))
    return extremum.value

# Generate points for plotting
def generate_points(n, x_min, x_max):
    x = np.linspace(x_min, x_max, n)
    y = f(x)
    return x, y

# Main script
if __name__ == "__main__":
    # Generate points for the function plot
    x_min, x_max = -np.pi, np.pi
    n_points = 1000
    x_vals, y_vals = generate_points(n_points, x_min, x_max)

    # Parameters for gradient descent and ascent
    step_size = 0.01
    tolerance = 1e-6
    delta = 1e-5

    # Find the minimum using the C function
    init_guess_min = 0.5
    minimum = find_extremum(init_guess_min, step_size, tolerance, delta, is_max=0)

    # Find the maximum using the C function
    init_guess_max = 0.0
    maximum = find_extremum(init_guess_max, step_size, tolerance, delta, is_max=1)

    # Plot the function
    plt.figure(figsize=(10, 6))
    plt.plot(x_vals, y_vals, label="$f(x) = |sin(4x) + 3|$", color="blue")

    # Highlight the minimum and maximum points found
    plt.scatter([minimum], [f(minimum)], color="red", label=f"Minimum (x={minimum:.2f}, f(x)={f(minimum):.2f})", zorder=5)
    plt.scatter([maximum], [f(maximum)], color="green", label=f"Maximum (x={maximum:.2f}, f(x)={f(maximum):.2f})", zorder=5)

    plt.title("Function Plot with Gradient Descent and Ascent")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend()
    plt.grid()
    plt.show()

    print(f"Local minimum found at x = {minimum}, f(x) = {f(minimum)}")
    print(f"Local maximum found at x = {maximum}, f(x) = {f(maximum)}")

