/*
Problem_17: Each MPI process generates data intended for every other process. 
Implement the communication using MPI_Alltoall and analyze its 
communication cost as the number of processes increases. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *sendbuf = malloc(size * sizeof(int));
    int *recvbuf = malloc(size * sizeof(int));

    for (int j = 0; j < size; j++)
        sendbuf[j] = rank * 100 + j;

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    MPI_Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - t1;

    for (int p = 0; p < size; p++) {
        if (p == rank) {
            printf("P%d received: ", rank);
            for (int i = 0; i < size; i++) printf("%d ", recvbuf[i]);
            printf("| Time: %f s\n", elapsed);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}