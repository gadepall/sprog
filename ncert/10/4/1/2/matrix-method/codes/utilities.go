package main

import (
    "math/cmplx"
    "fmt"
)
func MatScale(A [][]complex128, scaleFactor complex128) [][]complex128{
    n := len(A)
    resultMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        resultMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            resultMatrix[i][j] = scaleFactor * A[i][j]
        }
    }
    return resultMatrix
}
func MatAdd(A [][]complex128, B [][]complex128) [][]complex128 {
     n := len(A)
    resultMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        resultMatrix[i] = make([]complex128, n)
    }
     for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            resultMatrix[i][j] = A[i][j] + B[i][j]
        }
    }
    return resultMatrix           
}
func MatSubtract(A [][]complex128, B[][]complex128) [][]complex128{
    n := len(A)
    resultMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        resultMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            resultMatrix[i][j] = A[i][j] - B[i][j]
        }
    }
    return resultMatrix
}

func Hermitian(A [][]complex128) [][]complex128{
    n := len(A)
    resultMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        resultMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            resultMatrix[i][j] = cmplx.Conj(A[j][i])
        }
    }
    return resultMatrix
}

func Mnorm(vec []complex128) complex128{
    sum := complex(0, 0)
    for i := 0; i < len(vec); i++{
        sum += cmplx.Conj(vec[i]) * vec[i]
    }
    return cmplx.Sqrt(sum)
}

func OuterProduct(vec []complex128) [][]complex128{
    n := len(vec)
    productMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        productMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            productMatrix[i][j] = 2 * vec[i] * cmplx.Conj(vec[j])
        }
    }
    return productMatrix
}

func MatMultiply(A [][]complex128, B [][]complex128) [][]complex128{
    n := len(A)
    productMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        productMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            sum := complex(0, 0)
            for k := 0; k < n; k++{
                sum += A[i][k] * B[k][j]
            }
        productMatrix[i][j] = sum
        }
    }
    return productMatrix
}

func createIdentity(k int) [][]complex128{
    identity := make([][]complex128, k)
    for i := 0; i < k; i++{
        identity[i] = make([]complex128, k)
    }
    for i := 0; i < k; i++{
        identity[i][i] = complex(1, 0)
    }
    return identity
}

func norm(v []complex128) float64 {
    var sum float64
    for _, val := range v {
        sum += cmplx.Abs(val) * cmplx.Abs(val)
    }
    return sum
}

func printMatrix(matrix [][]complex128) {
    for _, row := range matrix {
        for _, value := range row {
            fmt.Printf("%6.9f +%6.9fi ", real(value), imag(value))
        }
        fmt.Println()
    }
}
