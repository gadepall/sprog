import numpy as np

def calculate_roots_with_eigenvalues(coefficients):
    """
    Calculate the roots of a polynomial by finding the eigenvalues of its companion matrix.

    Args:
    - coefficients: List or array of polynomial coefficients [a_n, ..., a_0].

    Returns:
    - roots: The roots of the polynomial.
    """
    # Normalize the polynomial coefficients to ensure the leading coefficient is 1
    coefficients = coefficients / coefficients[0]
    
    # For quadratic equation (ax^2 + bx + c = 0)
    if len(coefficients) == 3:  # This will work for a quadratic equation
        a, b, c = coefficients
        # Companion matrix for quadratic equation
        companion_matrix = np.array([[0, -c/a], [1, -b/a]])
        
        # Compute the eigenvalues of the companion matrix
        roots = np.linalg.eigvals(companion_matrix)
        return roots
    else:
        raise ValueError("This function currently supports only quadratic polynomials.")

# Coefficients of the polynomial 2x^2 + x - 4
coefficients = np.array([2, 1, -4])

# Compute roots
roots = calculate_roots_with_eigenvalues(coefficients)
print("Roots of the polynomial:", roots)

