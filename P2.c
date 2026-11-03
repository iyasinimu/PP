/*
Problem_2: The root process distributes different rows 
of a matrix to worker processes using MPI_Send. Workers
calculate the sum of their assigned rows and return the 
results using MPI_Recv.
*/
#include <stdio.h> 
#include <mpi.h> 

int main(int argc, char *argv[]) { 
    int rank, size, row[4], sum = 0, total = 0; 
    int mat[4][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}}; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    if (size != 4) { 
        if (rank == 0) printf("Run with 4 processes.\n"); 
        MPI_Finalize(); 
        return 0; 
    }

    MPI_Scatter(mat, 4, MPI_INT, row, 4, MPI_INT, 0, MPI_COMM_WORLD); 
    for (int i = 0; i < 4; i++) sum += row[i]; 

    printf("P%d: Row = %d %d %d %d Sum = %d\n", rank, row[0], row[1], row[2], row[3], sum); 

    MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 

    if (rank == 0) printf("Total matrix sum = %d\n", total); 

    MPI_Finalize(); 
    return 0; 
} 