#include <stdio.h>

void count_valid_numbers(int *total, int *div_by_5) {
    int digits[] = {0, 1, 3, 5, 7};
    int first, second, third, fourth;
    *total = 0;
    *div_by_5 = 0;
    
    for (int i = 0; i < 5; i++) {
        first = digits[i];
        // First digit must be 5 or 7 for the number to be greater than 5000
        if (first != 5 && first != 7) continue;
        
        for (int j = 0; j < 5; j++) {
            if (j == i) continue;
            second = digits[j];
            
            for (int k = 0; k < 5; k++) {
                if (k == i || k == j) continue;
                third = digits[k];
                
                for (int l = 0; l < 5; l++) {
                    if (l == i || l == j || l == k) continue;
                    fourth = digits[l];
                    
                    // Count only numbers greater than 5000
                    (*total)++;
                    // Check if the number is divisible by 5
                    if (fourth == 0 || fourth == 5) {
                        (*div_by_5)++;
                    }
                }
            }
        }
    }
}

