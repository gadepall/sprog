package main

import (
	"math/cmplx"
)

func solveJordanBlocks(A [][]complex128) {
    i := 0
    n := len(A)
    for i < n - 1{
       if cmplx.Abs(A[i+1][i]) > 1e-15 {
            trace := A[i][i] + A[i+1][i+1]
            det := (A[i][i] * A[i+1][i+1] - A[i][i+1] * A[i+1][i])
            A[i][i] = (trace + cmplx.Sqrt(trace * trace - 4 * det))/2
            A[i+1][i+1] = (trace - cmplx.Sqrt(trace * trace - 4 * det))/2
            i = i + 2
       } else {
           i += 1
       }
    }
}
