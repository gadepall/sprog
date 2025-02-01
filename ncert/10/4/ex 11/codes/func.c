#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

typedef struct
{
    double root1;
    double root2;
} roots;

double det(double matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

double trace(double matrix[2][2]) {
    return matrix[0][0] + matrix[1][1];
}

roots eigen_values(double a, double b, double c) {
    double matrix[2][2];
    matrix[0][0] = 0;
    matrix[1][0] = 1;
    matrix[0][1] = -c / a;
    matrix[1][1] = -b / a;

    roots root;
    double deter = det(matrix);
    double tr = trace(matrix);
    double discriminant = tr * tr - 4 * deter;
    printf("%.20lf\n",discriminant);
    if(discriminant<1e-10 && discriminant>-1e-10){
        discriminant=0;
    }
    if (discriminant < 0) {
        printf("Roots do not exist\n");
        exit(1);
    }
    
    root.root1 = (tr + sqrt(discriminant)) / 2;
    root.root2 = (tr - sqrt(discriminant)) / 2;

    return root;
}

