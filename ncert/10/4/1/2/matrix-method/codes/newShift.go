package main

func MshiftedIterations(A [][]complex128) ([]complex128, int){
    n := len(A)
    H := make([][]complex128, n)
    for i:= 0; i < n; i++{
        H[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            H[i][j] = A[i][j]
        } 
    }
    i := 0
    for i < 1000 {
        if isTriangular(H) == 1{
            break
        }
        sigmaI := MatScale(createIdentity(n), H[n-1][n-1])
        H = MatSubtract(H,sigmaI)    
        H = MatAdd(MapplyGivens(H), sigmaI)
        i++
    }
    solveJordanBlocks(H) 
    eigenValueList := make([]complex128, n)
    for i := 0; i < n; i++{
        eigenValueList[i] = H[i][i]
    }
    return eigenValueList, i 
}
