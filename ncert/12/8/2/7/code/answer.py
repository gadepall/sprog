import ctypes

# Load the shared library
lib = ctypes.CDLL('./verify.so')

# Specify the argument and return types for the area function
lib.Area.argtypes = [ctypes.c_int]  # Input: int
lib.Area.restype = ctypes.c_double  # Output: double

# Function to calculate the area in Python
def calculate_area(n):
    if n <= 1:
        raise ValueError("n must be greater than 1")
    return lib.Area(n)

# Main program
if __name__ == "__main__":
    try:
        # Input: Value of n
        n = int(input("Enter the value of n: "))
        
        # Calculate area using the shared library
        result = calculate_area(n)
        
        # Print the result
        print(f"The calculated area is: {result:.6f}")
    except ValueError as e:
        print(f"Error: {e}")

