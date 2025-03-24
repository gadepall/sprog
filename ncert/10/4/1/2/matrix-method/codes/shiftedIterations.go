package main

import (
    "math/cmplx"
    "fmt"
)
func isTriangular(A [][]complex128) int {
    n := len(A)
    for i := 0; i < n - 1; i++{
       if cmplx.Abs(A[i + 1][i]) > 1e-15{
           return 0
       } 
    }
    return 1
}
func shiftedIterations(A [][]complex128) ([]complex128, int){
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

    k := 0
    count := 0
    sigma_k := complex(0, 0)
    for m := n; m > 1; m--{
        for cmplx.Abs(H[m-1][m-2]) > 1e-15{
            if isTriangular(H) == 1{
                break
            }
            I := createIdentity(n)
            k += 1
            sigma_k = H[m-1][m-1]
            sigmaI := MatScale(I, sigma_k)
            toRotate := MatSubtract(H, sigmaI)
            fmt.Printf("Value returned by MapplyGivens at iteration %d: \n", count)
            H = MatAdd(MapplyGivens(toRotate), sigmaI)
            if count > 1000{
                break
            }
            count += 1

        }
    }
    solveJordanBlocks(H) 
    eigenValueList := make([]complex128, n)
    for i := 0; i < n; i++{
        eigenValueList[i] = H[i][i]
    }
    return eigenValueList, count
}
