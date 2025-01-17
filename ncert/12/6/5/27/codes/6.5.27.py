import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object file
lib = ctypes.CDLL('./6.5.27.so')

# Define the argument and return type of the function
lib.f.argtypes = [ctypes.c_double]
lib.f.restype = ctypes.c_double

# Gradient Descent Implementation
def gradient_descent(learning_rate, initial_y, max_iterations=1000, tolerance=1e-6):
    y = initial_y  # Initial guess for y
    for i in range(max_iterations):
        # Compute the gradient f'(y) = 2y - 8 using the derivative of the function
        gradient = 2 * y - 8
        if abs(gradient) < tolerance:  # Check for convergence
            break
        y = y - learning_rate * gradient  # Update y using the gradient descent rule
    
    return y

# Example usage: Use gradient descent to find the minimum of the function
initial_y = 0  # Initial guess
learning_rate = 0.1  # Learning rate
min_y = gradient_descent(learning_rate, initial_y)

# Compute the minimum function value using the loaded C function
min_value = lib.f(ctypes.c_double(min_y))

print(f"The minimum value of y is approximately: {min_y}")
print(f"The minimum value of the function f(y) is approximately: {min_value}")

# Plot the function and the minimum
y_values = np.linspace(-10, 10, 400)
f_values = np.array([lib.f(ctypes.c_double(y)) for y in y_values])

plt.plot(y_values, f_values, label='f(y) = y^2 + 25 - 8y')
plt.scatter(min_y, min_value, color='red', zorder=5,label='min')
plt.text(min_y, min_value, f"  Min: ({min_y:.2f}, {min_value:.2f})", fontsize=12, verticalalignment='bottom')
plt.xlabel('y')
plt.ylabel('f(y)')
plt.title(' ')
plt.legend()
plt.grid(True)
plt.savefig("6.5.27.png")
plt.show()

