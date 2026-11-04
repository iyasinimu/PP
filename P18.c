/*
Problem_18: Divide a large dataset among MPI processes. Each process generates a local histogram, 
and MPI_Allreduce combines all local histograms into a global histogram. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATA_SIZE 16
#define NUM_BINS 5

int main(int argc, char *argv[]) {
    int rank, size;
    int dataset[DATA_SIZE] = {0, 1, 2, 3, 4, 1, 2, 3, 0, 4, 2, 3, 1, 0, 4, 2};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = DATA_SIZE / size;
    int local_data[local_n];
    int local_hist[NUM_BINS] = {0}, global_hist[NUM_BINS] = {0};

    MPI_Scatter(dataset, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
        if (local_data[i] >= 0 && local_data[i] < NUM_BINS)
            local_hist[local_data[i]]++;

    MPI_Allreduce(local_hist, global_hist, NUM_BINS, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global Histogram:\n");
        for (int i = 0; i < NUM_BINS; i++)
            printf("Bin %d : %d\n", i, global_hist[i]);
    }

    MPI_Finalize();
    return 0;
}