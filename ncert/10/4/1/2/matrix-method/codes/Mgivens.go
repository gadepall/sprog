package main

import(
    "math/cmplx"
    "math"
    //"fmt"
)

func Mgivens(A [][]complex128, k int) (complex128, complex128){
    c_k := complex(0, 0)
    s_k := complex(0, 0)
    f := A[k][k]
    g := A[k + 1][k]
    if math.Abs(cmplx.Abs(f)) < 1e-15 && math.Abs(cmplx.Abs(g)) < 1e-15{
        return complex(1, 0) , complex(0, 0)
    } else {
        denom := complex(math.Sqrt(math.Abs(cmplx.Abs(f))*math.Abs(cmplx.Abs(f)) + math.Abs(cmplx.Abs(g))*math.Abs(cmplx.Abs(g))), 0)
        c_k = cmplx.Conj(f)/denom
        s_k = cmplx.Conj(g)/denom
        return c_k, s_k
    }
}

func MapplyGivens(A [][]complex128) [][]complex128{
    n := len(A)
    Q := createIdentity(n)
    workingMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        workingMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            workingMatrix[i][j] = A[i][j]
        }
    }
    for k := 0; k < n - 1; k++{
        G_k := createIdentity(n)
        c_k, s_k := Mgivens(workingMatrix, k)
        G_k[k][k] = c_k
        G_k[k + 1][k + 1] = cmplx.Conj(c_k)
        G_k[k + 1][k] = -cmplx.Conj(s_k)
        G_k[k][k + 1] = s_k
        Q = MatMultiply(G_k, Q)
        workingMatrix = MatMultiply(G_k, workingMatrix)
    }
    workingMatrix = MatMultiply(workingMatrix, Hermitian(Q))
    return workingMatrix
}
