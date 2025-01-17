package main

import (
	"math/cmplx"
)

func Mhessenberg(A [][]complex128) [][]complex128{
    n := len(A)
    H := make([][]complex128, n)
    for i := 0; i < n; i++{
        H[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            H[i][j] = A[i][j]
        }
    } 

    for k := 0; k < n - 2; k++{
        u := make([]complex128, n - k - 1)
        for i := 0; i < n-k-1; i++{
            u[i] = H[k + i +1][k]
        }
        normU := Mnorm(u)
        if cmplx.Abs(normU) < 1e-15{
            continue
        }
        if cmplx.Abs(u[0]) > 1e-15 {
            u[0] += (u[0] / complex(cmplx.Abs(u[0]), 0)) * normU
        } else {
            u[0] += normU
        }
        normU = Mnorm(u)
        for i := 0; i < len(u); i++ {
            u[i] /= normU
        }
        P_k := createIdentity(n)
        smallIdentity := createIdentity(len(u))

        U := OuterProduct(u)

        P_small := MatSubtract(smallIdentity, U)
        for row := k + 1; row < n; row++{
            for column := k + 1; column < n; column++{
                P_k[row][column] = P_small[row - k - 1][column - k - 1]
            }
        }
        H = MatMultiply(P_k, H)
        H = MatMultiply(H, P_k)
    }
    return H
}
