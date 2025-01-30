#include<stdio.h>
#include<complex.h>
#include<math.h>
#include<stdlib.h>

//variable to count number of iterations taken to converge to upper triangular form.
int count = 0;

//function to check if the matrix is triangular
int is_triangular(int n, double complex A[n][n], double tolerance){
    for(int i=0;i<n-1;i++){
        if(cabs(A[i+1][i]) > tolerance){
            return 0;
        }
    }
    return 1;
}

//function to convert a matrix into an identity matrix
void mat_id(int n, double complex A[n][n]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i == j){
                A[i][j] = 1;
            }
            else{
                A[i][j] = 0;
            }
        }
    }
}

//function to scale the elements of a matrix
void mat_scale(int n, double complex A[n][n], double complex scale){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            A[i][j] *= scale;
        }
    }
}

//function to multiply 2 matrices
void mat_mul(int n, double complex A[n][n], double complex B[n][n], double complex C[n][n]){
    double complex sum = 0 + 0*I;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                sum += (A[i][k] * B[k][j]);
            }
            C[i][j] = sum;
            sum = 0;
        }
    }
}

//function to produce transpose of a matrix
void mat_transpose(int n, double complex A[n][n], double complex B[n][n]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            B[i][j] = conj(A[j][i]);
        }
    }
}

//function to print a matrix
void mat_print(int n, double complex A[n][n]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("(%lf) + i(%lf)      ",creal(A[i][j]),cimag(A[i][j]));
        }
        printf("\n");
    }
    printf("\n");
}

//function to scale a column of a matrix
void column_scale(int n, double complex A[n][n], int column, double complex scale){
    for(int i=0;i<n;i++){
        A[i][column] = A[i][column]*scale;
    }
}

//function to calculate the euclidean norm of a vector
double vector_norm(int n, double complex v[n]){
    double pro = 0;
    for(int i=0;i<n;i++){
        pro += v[i]*conj(v[i]);
    }
    return sqrt(pro);
}

//function to find inner product of 2 columns of 2 matrices
double complex vector_dot(int n, double complex A[n][n], double complex B[n][n], int columnA, int columnB){
    double complex pro = 0 + 0*I;
    for(int i=0;i<n;i++){
        pro += A[i][columnA] * conj(B[i][columnB]);
    }
    return pro;
}

//function to find outer product of a vector with itself
void outer_product(int n, double complex v[n], double complex p[n][n]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            p[i][j] = v[i] * conj(v[j]);
        }
    }
}

//function to convert a matrix to its upper Hesserberg form through Householder reflections
void householder_tranform(int n, double complex A[n][n]){
    double complex* x, H[n][n];
    int index = 0;
    for(int i=0;i<n-2;i++){
        mat_id(n,H);
        x = (double complex*)malloc(sizeof(double complex)*(n-i-1));
        index = 0;
        //x vector is the i_th column of A matrix
        for(int j=i+1;j<n;j++){
            x[index] = A[j][i];
            index++;
        }
        double complex rho;
        if(cabs(x[0]) <= 1e-6){
            rho = 0;
        }
        else{
            rho = -x[0]/cabs(x[0]);
        }
        //The first element of the x vector is subtracted by rho * norm of the vector itself and is then normalised
        double norm = vector_norm(n-i-1,x);
        x[0] -= rho * norm;
        double new_norm = vector_norm(n-i-1,x);
        if(new_norm > 1e-10){
            for(int j=0;j<n-i-1;j++){
                x[j] /= new_norm;
            }
        }
        //p matrix is defined as I - 2xx^T
        double complex p[n-i-1][n-i-1];
        outer_product(n-i-1,x,p);
        mat_scale(n-i-1,p,-2);
        for(int j=0;j<n-i-1;j++){
            p[j][j] += 1;
        }
        //H matrix of size n is defined as an identity matrix with a block of the p matrix within it
        int index1 = 0, index2 = 0;
        for(int j=i+1;j<n;j++){
            for(int k=i+1;k<n;k++){
                H[j][k] = p[index1][index2];
                index2++;
            }
            index2 = 0;
            index1++;
        }
        double complex temp[n][n];
        //the new A is defined as HAH
        mat_mul(n,H,A,temp);
        mat_mul(n,temp,H,A);
        free(x);
    }
}

//function to decompose a matrix into its QR form through Gram Schmidt algorithm
void qr_gram_schmidt(int n, double complex A[n][n], double complex Q[n][n], double complex R[n][n]){
    //Q matrix starts off as A matrix itself
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            Q[i][j] = A[i][j];
        }
    }
    int check = 0;
    for(int i=0;i<n;i++){
        if(cabs(Q[i][0]) >= 1e-6){
            check = 1;
            break;
        }
    }
    //if the column is not a zero column, normalize it
    if(check == 1){
        column_scale(n,Q,0,(1.0/sqrt(vector_dot(n,Q,Q,0,0))));
    }
    for(int j=1;j<n;j++){
        for(int i=0;i<j;i++){
            //scale is the dot product of the j_th column of A matrix with the i_th column of Q matrix
            double complex scale = vector_dot(n,A,Q,j,i);
            //j_th column of Q is made lineraly independent of all the previous columns by subtrating their projection onto this column
            for(int k=0;k<n;k++){
                Q[k][j] -= (scale * Q[k][i]);
            }
        }
        //check to see if Q is a zero column, i.e., it is linearly dependent of previous columns, if not then normalize it
        check = 0;
        for(int i=0;i<n;i++){
            if(cabs(Q[i][j]) >= 1e-6){
                check = 1;
                break;
            }
        }
        if(check == 1){
            column_scale(n,Q,j,(1.0/sqrt(vector_dot(n,Q,Q,j,j))));
        }
    }
    double complex Q_T[n][n];
    mat_transpose(n,Q,Q_T);
    //R matrix is Q^T A
    mat_mul(n,Q_T,A,R); 
}

//function to decompose a matrix into its QR form through Givens rotations to zero out the sub-diagonal elements
void givens_rotation(int n, double complex A[n][n], double complex Q[n][n], double complex R[n][n]){
    double complex G[n][n], G_T[n][n], temp[n][n];
    //A is copied onto R
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            R[i][j] = A[i][j];
        }
    }
    //Q starts off as an identity matrix
    mat_id(n,Q);
    for(int i=0;i<n-1;i++){
        //The G matrix is formed
        double complex x = conj(R[i][i]), y = conj(R[i+1][i]);
        double r = sqrt(pow(cabs(x),2) + pow(cabs(y),2));
        mat_id(n,G);
        if(r > 1e-10){
            G[i][i] = x/r;
            G[i][i+1] = y/r;
            G[i+1][i] = -conj(y)/r;
            G[i+1][i+1] = conj(x)/r;
        }
        //if r is close to zero that means the sub-diagonal element is already 0 so this iteration is ignored
        else{
            continue;
        }
        mat_transpose(n,G,G_T);
        //transpose of the G matrix is multiplied to Q on the right and the result is the new Q matrix
        mat_mul(n,Q,G_T,temp);
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                Q[j][k] = temp[j][k];
            }
        }
        //G matrix is multiplied to R on the left and the result is the new R matrix
        mat_mul(n,G,R,temp);
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                R[j][k] = temp[j][k];
            }
        }
    }
}

//function to find Eigenvalues of a matrix by iterating and converting the matrix into triangular form
void eigenvalues(int n, int max_iterations, double complex A[n][n], double tolerance){
    for(int i=0;i<max_iterations;i++){
        double complex Q[n][n], R[n][n];
        //if matrix has already converged, stop the iterations
        if(is_triangular(n,A,tolerance)){
            return;
        }
        //shift is implemented to help increase rate of convergence
        double complex sigma = A[n-1][n-1];
        for(int k=0;k<n;k++){
            A[k][k] -= sigma;
        }
        //A is factored into QR and then R and Q are multiplied to get new A
        givens_rotation(n,A,Q,R);
        mat_mul(n,R,Q,A);
        for(int k=0;k<n;k++){
            A[k][k] += sigma;
        }
        count++;
    }
}

int main(){
    printf("This is the program to calculate all the Eigenvalues of a general complex matrix through the iterative QR algorithm:\n");
    printf("Enter the size of the matrix: ");
    //size of the matrix is taken as input
    int n;
    scanf("%i",&n);
    //if size is zero there is no matrix, hence no Eigenvalues
    if(n == 0){
        printf("No eigen values");
        return 0;
    }
    printf("In the next %i lines, enter the elements of the matrix in the form of R I (row wise)\n",n);
    //elements of matrix are taken as input
    double complex A[n][n];
    double real, imag;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            scanf("%lf %lf",&real,&imag);
            A[i][j] = real + I*imag;
        }
    }
    //Entered martix is displayed for verification
    printf("Entered matrix is:\n");
    mat_print(n,A);
    //if matrix is 1x1 the Eigenvalue is the element itself
    if(n == 1){
        printf("Eigen value is:\n(%lf) + i(%lf)",creal(A[0][0]),cimag(A[0][0]));
        return 0;
    }
    //matrix is converted to upper Hesserberg form
    householder_tranform(n,A);
    //The matrix is converged to upper triangular form and displayed with the number of itrations taken to converge
    eigenvalues(n,1000,A,1e-10);
    printf("The final matrix after %i number of iterations of the QR iterative algorithm is:\n\n",count);
    mat_print(n,A);
    //The diagonal elements are read off as the Eigenvalues with the exception in the cases of Jordan blocks.
    double complex eigen[n];
    for(int i=0;i<n-1;i++){
        if(cabs(A[i+1][i]) <= 1e-6){
            eigen[i] = A[i][i];
        }
        else{
            //Eigen values of Jordan blocks are found by solving the quadratic equation for the 2x2 block
            double complex b = -(A[i][i] + A[i+1][i+1]);
            double complex c = (A[i][i] * A[i+1][i+1]) - (A[i+1][i] * A[i][i+1]);
            double complex D = csqrt(b*b - 4*c);
            eigen[i] = (-b + D)/2;
            eigen[i+1] = (-b - D)/2;
            i++;
        }
    }
    eigen[n-1] = A[n-1][n-1];
    //Eigenvalues are diplayed
    printf("Eigenvalues are:\n\n");
    for(int i=0;i<n;i++){
        printf("Value %i:  (%lf) + i(%lf)\n",i+1,creal(eigen[i]),cimag(eigen[i]));
    }

    return 0;
}