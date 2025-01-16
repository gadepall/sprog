package main

import(
    "fmt"
)

func main(){

    /*
    matrix := [][]complex128{
        {complex(5, 1), complex(9, 2), complex(3, 9), complex(5, 3), complex(1, 1)},
        {complex(3, 2), complex(3, 5), complex(0, 8), complex(2, 5), complex(2, 4)},
        {complex(6, 3), complex(2, 6), complex(8, 7), complex(9, 6), complex(3, 6)},
        {complex(8, 4), complex(7, 7), complex(1, 6), complex(8, 7), complex(4, 8)},
        {complex(5, 5), complex(2, 8), complex(6, 5), complex(7, 8), complex(5, 8)},
    }
    matrix := [][]complex128{
        {complex(5, 1), complex(9, 6), complex(3, 9), complex(5, 8)},
        {complex(3, 2), complex(3, 7), complex(0, 7), complex(2, 6)},
        {complex(6, 3), complex(2, 8), complex(8, 5), complex(9, 4)},
        {complex(8, 4), complex(7, 9), complex(1, 3), complex(8, 2)},
    }

    matrix := [][]complex128{
        {complex(5, 0), complex(9, 0), complex(3, 0), complex(5, 0)},
        {complex(3, 0), complex(3, 0), complex(0, 0), complex(2, 0)},
        {complex(6, 0), complex(2, 0), complex(8, 0), complex(9, 0)},
        {complex(8, 0), complex(7, 0), complex(1, 0), complex(8, 0)},
    }
    matrix := [][]complex128{
        {complex(5, 1), complex(9, 6), complex(3, 2), complex(5, 1), complex(1, 1)},
        {complex(3, 2), complex(3, 7), complex(0, 4), complex(2, 3), complex(2, 4)},
        {complex(6, 3), complex(2, 8), complex(8, 6), complex(9, 5), complex(3, 7)},
        {complex(8, 4), complex(7, 9), complex(1, 8), complex(8, 7), complex(4, 2)},
        {complex(5, 5), complex(2, 0), complex(6, 0), complex(7, 9), complex(5, 5)},

    }
    matrix := [][]complex128{
        {complex(5, 1), complex(9, 6), complex(3, 2), complex(5, 1), complex(1, 1)},
        {complex(3, 2), complex(3, 7), complex(0, 4), complex(2, 3), complex(2, 4)},
        {complex(6, 3), complex(2, 8), complex(8, 6), complex(9, 5), complex(3, 7)},
        {complex(8, 4), complex(7, 9), complex(1, 8), complex(8, 7), complex(4, 2)},
        {complex(5, 5), complex(2, 0), complex(6, 0), complex(7, 9), complex(5, 5)},
    }
    */
    /*
    matrix := [][]complex128{
        {complex(2, 0), complex(3, 0), complex(5, 0), complex(7, 0), complex(11, 0)},
        {complex(13, 0), complex(17, 0), complex(19, 0), complex(23, 0), complex(29, 0)},
        {complex(31, 0), complex(37, 0), complex(41, 0), complex(43, 0), complex(47, 0)},
        {complex(53, 0), complex(59, 0), complex(61, 0), complex(67, 0), complex(71, 0)},
        {complex(73, 0), complex(79, 0), complex(83, 0), complex(89, 0), complex(97, 0)},
    }
    */ 
    matrix := [][]complex128{
      {complex(0, 0), complex(-6, 0)},
      {complex(1, 0), complex(4, 0)},
    }

    hessenbergMatrix := Mhessenberg(matrix)
    fmt.Println("Upper Hessenberg Matrix")
    printMatrix(hessenbergMatrix)

    eigenValueList, _:= MshiftedIterations(hessenbergMatrix)

    for i := 0; i < len(eigenValueList); i++{
        fmt.Printf("eigenvalue %d: %6.6f + %6.6fi\n", i + 1, real(eigenValueList[i]), imag(eigenValueList[i]))
    }
}
