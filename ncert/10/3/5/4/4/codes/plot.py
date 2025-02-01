import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared object (.so) file
libcode = ctypes.CDLL('./libcode.so')

# Create variables to hold the results
v1 = ctypes.c_double()
v2 = ctypes.c_double()

# Call the function to compute car speeds
libcode.get_car_speeds(ctypes.byref(v1), ctypes.byref(v2))

# Print the results
print(f"Speed of car 1 (v1): {v1.value} km/h")
print(f"Speed of car 2 (v2): {v2.value} km/h")

# Plot the equations (same as before)

# Define the equations
def equation_1(v1, v2):
    return v1 - v2 - 20  # v1 - v2 = 20

def equation_2(v1, v2):
    return v1 + v2 - 100  # v1 + v2 = 100

# Create a range for v1 and v2
v1_values = np.linspace(0, 120, 200)
v2_values_1 = v1_values - 20  # From equation v1 - v2 = 20
v2_values_2 = 100 - v1_values  # From equation v1 + v2 = 100

# Plot both equations
plt.plot(v1_values, v2_values_1, label=r'$v_1 - v_2 = 20$', color='blue')
plt.plot(v1_values, v2_values_2, label=r'$v_1 + v_2 = 100$', color='red')

# Mark the intersection point
plt.scatter(v1.value, v2.value, color='green', label=f'Solution: v1={v1.value}, v2={v2.value}')

# Add labels and title
plt.xlabel(r'$v_1$ (Speed of car from A)')
plt.ylabel(r'$v_2$ (Speed of car from B)')
plt.title('Plot of Car Speed Equations')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()

