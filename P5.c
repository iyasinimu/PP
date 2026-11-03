/*
Problem_5: Distribute matrix rows from the root process 
to other processes using MPI_Send. Each process performs 
its portion of matrix multiplication and sends the result 
back to the root.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 4

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) printf("N (%d) must be divisible by process count (%d).\n", N, size);
        MPI_Finalize();
        return 0;
    }

    int rows_per_proc = N / size;
    int A[N][N], B[N][N], C[N][N];
    int local_A[rows_per_proc][N], local_C[rows_per_proc][N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j + 1;
                B[i][j] = (i == j) ? 1 : 2;
            }
        }
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rows_per_proc * N, MPI_INT, local_A, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i][j] = 0;
            for (int k = 0; k < N; k++)
                local_C[i][j] += local_A[i][k] * B[k][j];
        }
    }

    MPI_Gather(local_C, rows_per_proc * N, MPI_INT, C, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result Matrix C (%dx%d):\n", N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) printf("%d ", C[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}