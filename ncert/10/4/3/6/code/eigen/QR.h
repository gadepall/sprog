double complex** grMat(double complex** A, int i, int j){
    double complex** G = createMat(2,2);
    double complex r = 1/sqrt(cabs(A[i][j])*cabs(A[i][j]) + cabs(A[i-1][j])*cabs(A[i-1][j]));
    double complex s = r*A[i][j];
    double complex c = r*A[i-1][j];
    // printf("%lf\n",creal(r));
    G[1][1] = conj(c);
    G[0][0] = (c);
    G[1][0] = -(s);
    G[0][1] = conj(s);

    return G;
}

void GMatmul(double complex** G, double complex** R, int m,int i){
    double complex temp;
    for(int j=0;j<m;j++){
        temp = G[0][0]*R[i-1][j] + G[0][1]*R[i][j];
        R[i][j] = G[1][0]*R[i-1][j] + G[1][1]*R[i][j];
        R[i-1][j] = temp;
    }
}

void GTMatMul(double complex** Gt,  double complex** Q, int m, int i){
    double complex temp;
    for(int j=0;j<m;j++){
        temp = Q[j][i-1]*Gt[0][0] + Q[j][i]*Gt[0][1];
        Q[j][i] = Q[j][i-1]*Gt[1][0] + Q[j][i]*Gt[1][1];
        Q[j][i-1] = temp;
    }
}

void hessenberg(double complex** A, int m, double complex*** Hq){
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    double complex** q = eye(m);
    double complex** H = createMat(m,m);
    copyMat(A, H, m , m);
    int conv = 1;
    double complex** G; double complex** Gt;
    // #pragma omp for
    for(int i=0;i<m-2;i++){
        for(int j=m-1;j>i+1;j--){
            if(cabs(H[j][i])<1.0e-7) continue;
            else{
                G = grMat(H, j, i);
                // Gt = transpose(G, 2, 2);
                GTMatMul(G, q, m, j);
                GMatmul(G, H, m, j);
                GTMatMul(G, H, m, j);
                freeMat(G,2);
                //freeMat(Gt,2);
                conv = 0;
            }
        }
    }
    // Hq[0] = H; Hq[1] = q;
    // freeMat(Q,m); freeMat(R,m);
    // memcpy(Hq[0], H, m*sizeof(double complex*));
    // memcpy(Hq[1], q, m*sizeof(double complex*));
    copyMat(H, Hq[0], m , m);
    copyMat(q, Hq[1], m, m);
    freeMat(H, m);
    freeMat(q, m);
    end = clock();
    printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);


}

double complex wilkinsonCooef(double complex** A, int m){
    double complex** Asl = createMat(2,2);
    Asl[0][0] = A[m-2][m-2]; Asl[0][1] = A[m-2][m-1];
    Asl[1][0] = A[m-1][m-2]; Asl[1][1] = A[m-1][m-1];
    double complex* eig = eigs2x(Asl);
    double complex v;
    if(!cimag(eig[0])){
        v =  ((abs(eig[0])-creal(A[m-1][m-1]))<(abs(eig[1])-creal(A[m-1][m-1])))?eig[0]:eig[1];
    }
    else{
        v = creal(eig[0]);
    }
    free(eig);
    freeMat(Asl, 2);
    return v;
    // double complex sig = (A[m-2][m-2] - A[m-1][m-1])/2
    // double complex mu = A[m-1][m-1] - (sig/cabs(sig))*()
}

#pragma omp parallel num_threads(4)
void QRdec(double complex** A, int m, double complex*** QR){
    // clock_t start, end;
    // double cpu_time_used;
    // start = clock();
    double complex** Q = eye(m);
    double complex** R = createMat(m ,m);
    copyMat(A, R, m, m);
    int conv = 1;
    double complex** G; double complex** Gt;
    // #pragma omp for
    for(int i=0;i<m-1;i++){
        // #pragma omp for
        // for(int j=m-1;j>=i+(i%2);j-=2){
        //     if(cabs(R[j][i])<1.0e-9) continue;
        //     else{
        //         G = grMat(R, j, i);
        //         // Gt = transpose(G, 2, 2);
        //         GTMatMul(G, Q, m, j);
        //         GMatmul(G, R, m, j);
        //         freeMat(G,2);
        //         //freeMat(Gt,2);
        //         conv = 0;
        //     }
        // }
        // // printf("bal\n");
        // #pragma omp for
        // for(int j=m-2;j>=i+((i%2)?0:1);j-=2){
        //     // printf("%d\n",j);
        //     if(cabs(R[j][i])<1.0e-9) continue;
        //     else{
        //         G = grMat(R, j, i);
        //         // Gt = transpose(G, 2, 2);
        //         GTMatMul(G, Q, m, j);
        //         GMatmul(G, R, m, j);
        //         freeMat(G,2);
        //         //freeMat(Gt,2);
        //         conv = 0;
        //     }
        
        // }
        for(int j=m-1;j>i;j--){
            // printf("%d\n",j);
            if(cabs(R[j][i])<1.0e-9) continue;
            else{
                G = grMat(R, j, i);
                // Gt = transpose(G, 2, 2);
                GTMatMul(G, Q, m, j);
                GMatmul(G, R, m, j);
                freeMat(G,2);
                //freeMat(Gt,2);
                conv = 0;
            }
        
        }   
    }
    QR[0] = Q; QR[1] = R;
    // freeMat(Q,m); 
    // free(R);
    // end = clock();
    // printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);

}

double complex* eigs(double complex** A, int m){
    double complex* eigs = malloc(m * sizeof(double complex));
    double complex* eigs2;
    double complex** Asl = createMat(2 ,2);
    int i=0;
    while(i<m){
    // printf("yy %d\n",i);
        if(i==m-1 || cabs(A[i][i+1]*(A[i+1][i]))<10.0e-10) eigs[i] = A[i][i];
        else{
            Asl[0][0] = A[i][i]; Asl[0][1] = A[i][i+1];
            Asl[1][0] = A[i+1][i]; Asl[1][1] = A[i+1][i+1];

            eigs2 = eigs2x(Asl);
            eigs[i] = eigs2[0]; eigs[i+1] = eigs2[1];
            i++;
            free(eigs2);
        }
        i++;
    }
    freeMat(Asl, 2);
    return eigs;
}

#pragma omp parallel num_threads(4)
int checkConv(double complex** Ak, double complex** AkP, int m){
    double complex change=0+0*I;
    double complex* e1 = eigs(Ak, m);
    double complex* e2 = eigs(AkP, m);
    #pragma omp for
    for(int i=0;i<m;i++){
        // change += (AkP[i][i] - Ak[i][i])*(AkP[i][i] - Ak[i][i]);
        change += (cabs(e1[i]) - cabs(e2[i]))*(cabs(e1[i]) - cabs(e2[i]));
        // for(int j=m-1;j>i+1;j--){
        //     change+=AkP[j][i]-Ak[j][i];
        //     // printf("ll %lf %lf\n",cabs(change), creal(AkP[j][i]-Ak[j][i]));

        // }
        // change += AkP[i][i] - Ak[i][i];

    }
    // printf("%lf\n",creal(change));
    // printMat(Ak,m,m);printf("\n");
    // printMat(AkP,m,m);printf("\n");
    // for(int i=0;i<m;i++) printf("%lf ",creal(e1[i]));
    // printf("\n");
    // for (int i = 0; i < m; i++) printf("%lf ", creal(e2[i]));
    // printf("\n");
    free(e1);free(e2);
    if(cabs(change)<1.0e-10) return 1;
    return 0;
}

double complex** QReig(double complex** A, int m){
    double complex** Ak = createMat(m,m);
    double complex** AkP = createMat(m ,m); 
    copyMat(A, Ak, m ,m );
    // memcpy(Ak, A, m * sizeof(double complex*));
    // memcpy(AkP, A, m * sizeof(double complex*));
    copyMat(Ak, AkP, m, m);
    double complex*** QR = malloc(2 * sizeof(double complex**));
    // QR *qr = malloc(sizeof(QR));
    int conv = 0;
    int iterations=0;
    for(int i=0;conv!=1 && i<m*m*m;i++){
    // for(int i=0;i <1000;i++){
        // printf("%d\n",i);
        iterations++;
        // double complex wilk = wilkinsonCooef(Ak, m);
        // double complex** shif = eye(m);
        // scale(shif, -wilk, m , m, shif);
        // matAdd(Ak, shif, m, m, Ak);
        QRdec(Ak, m, QR);
        matMul(QR[1],QR[0], m, m, m, Ak);
        // scale(shif, -1, m , m, shif);
        // matAdd(Ak, shif, m , m, Ak);
        conv = checkConv(Ak, AkP, m);
        copyMat(Ak, AkP, m , m);
        // printf("ballin\n");
        // printMat(QR[1],m,m);
        // freeMat(shif, m);
        freeMat(QR[0],m);
        freeMat(QR[1],m);
        // printf("ja\n");
    }
    freeMat(AkP, m);
    if(QR!=NULL) free(QR);
    printf("%d\n",iterations);
    return Ak;
}


