package main

func runIterations(A [][]complex128, k int) []complex128{
    n := len(A)
    workingMatrix := make([][]complex128, n)
    for i := 0; i < n; i++{
        workingMatrix[i] = make([]complex128, n)
    }
    for i := 0; i < n; i++{
        for j := 0; j < n; j++{
            workingMatrix[i][j] = A[i][j]
        }
    }
    for i := 0; i < k; i++{
        workingMatrix = MapplyGivens(workingMatrix)
    }
    solveJordanBlocks(workingMatrix)
    eigenValueList := make([]complex128, n)
    for i := 0; i < len(workingMatrix); i++{
        eigenValueList[i] = workingMatrix[i][i]
    }
    return eigenValueList

}
