/*
Problem_3: Implement a ring topology in which every MPI 
process sends a message to its next process and receives 
a message from its previous process. Measure the 
communication time for different message sizes.
*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int token = 0;

    if (rank != 0) {
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d receives %d from P%d\n", rank, token, rank-1);
    } 
    else token = 100;

    printf("P%d sends %d to P%d\n", rank, token, (rank+1)%size);
  
    MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d receives %d from P%d\n", rank, token, size-1);
    }

    MPI_Finalize();
    return 0;
}
