import ctypes
from ctypes import c_double, Structure

# Load the compiled shared library
lib = ctypes.CDLL('./solver.so')

# Define a struct for complex numbers (to match the C `double complex`)
class Complex(ctypes.Structure):
    _fields_ = [("real", c_double), ("imag", c_double)]

# Define the `randomgen` function's argument and return types
lib.randomgen.restype = c_double
lib.randomgen.argtypes = [c_double, c_double]

# Define the `newton_raphson` function's argument and return types
lib.newton_raphson.restype = Complex  # Specify it returns a `Complex` struct
lib.newton_raphson.argtypes = [
    c_double, c_double,  # Coefficients `a` (real and imaginary parts)
    c_double, c_double,  # Coefficients `b` (real and imaginary parts)
    c_double, c_double   # Coefficients `c` (real and imaginary parts)
]

# Define coefficients of the quadratic equation
a_real, a_imag = 2.0, 0.0
b_real, b_imag = 1.0, 0.0
c_real, c_imag = -4.0, 0.0

# Compute the first root
root1 = lib.newton_raphson(
    a_real, a_imag,
    b_real, b_imag,
    c_real, c_imag
)

# Perturb coefficients slightly to find a different root (optional)
b_real_alt = b_real + 1e-3

# Compute the second root
root2 = lib.newton_raphson(
    a_real, a_imag,
    b_real_alt, b_imag,
    c_real, c_imag
)

# Extract the real and imaginary parts for both roots
root1_complex = complex(root1.real, root1.imag)
root2_complex = complex(root2.real, root2.imag)

# Print the computed roots
print("Roots of the quadratic equation using Newton-Raphson:")
print(f"Root 1: {root1_complex.real:.4f} + {root1_complex.imag:.4f}j")
print(f"Root 2: {root2_complex.real:.4f} + {root2_complex.imag:.4f}j")
