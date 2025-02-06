#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void verify_expressions() {
    int A, B;
    srand(time(NULL)); // Seed the random number generator to ensure different outputs on each run
    
    for (int i = 0; i < 5; i++) { // Run multiple tests
        A = rand() % 2; // Generate a random binary value for A (0 or 1)
        B = rand() % 2; // Generate a random binary value for B (0 or 1)
        
        // Calculate AB + A'B using bitwise operators
        int expr1 = (A & B) | (~A & B);  
        
        // Calculate AB + AB' using bitwise operators
        int expr2 = (A & B) | (A & ~B);  
        
        // Calculate (A + B) using bitwise OR
        int expr3 = (A | B);             
        
        // Calculate AB + A'B + AB' using bitwise operators
        int rhs3 = (A & B) | (~A & B) | (A & ~B); 
        
        // Check if all expressions evaluate correctly
        if (expr1 == B && expr2 == A && expr3 == rhs3) {
            printf("A = %d, B = %d -> Verified\n", A, B); // Print verified if expressions match expected values
        } else {
            printf("A = %d, B = %d -> Not Verified\n", A, B); // Print not verified if there's a mismatch
        }
    }
}

int main() {
    verify_expressions(); // Call function to verify expressions
    return 0; // Return 0 to indicate successful execution
}
