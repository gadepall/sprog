import ctypes
import numpy as np
import matplotlib.pyplot as plt
import os

# Get the directory where the current code is located
script_dir = os.path.dirname(os.path.abspath(__file__))
# path we want to save this figure 
save_directory = os.path.join(script_dir, "../fig")

if not os.path.exists(save_directory):
    os.makedirs(save_directory)  # creates directory if not exists

# Load the shared library (make sure the path is correct)
lib = ctypes.CDLL('./test.so')  # Use the appropriate path for your OS

# Define the argument types of the C function
lib.lu_decomposition.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # Pointer to matrix A
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),  # Pointer to matrix L
    ctypes.POINTER(ctypes.POINTER(ctypes.c_double))   # Pointer to matrix U
]

# Initialize the matrices in NumPy
A = np.array([[3, 4], [2, -2]], dtype=np.float64)
L = np.zeros((2, 2), dtype=np.float64)
U = np.zeros((2, 2), dtype=np.float64)

# Convert NumPy arrays to ctypes pointers
A_ptr = A.ctypes.data_as(ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
L_ptr = L.ctypes.data_as(ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
U_ptr = U.ctypes.data_as(ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))

# Call the C function to perform LU decomposition
lib.lu_decomposition(A_ptr, L_ptr, U_ptr)

x1 = np.linspace(0,5,100)
y1 = x1 - 1

plt.plot(x1,y1,label="2x-2y=2",color = "red")
x2 = np.linspace(0,5,100)
y2 = (10 - 3*x2)/4

plt.plot(x2,y2,label="3x+4y=10", color = "blue")
plt.plot(2, 1, label="Intersection point (2,1)", color="purple", marker='o', markersize=8)


plt.grid()
plt.axis("equal")
plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.legend()

# Save the combined figure
save_path = os.path.join(save_directory, 'combined_fig.jpg')  # saves as combined_fig.jpg
plt.savefig(save_path)
plt.show()


