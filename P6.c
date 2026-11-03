/*
Problem_6: Divide a one-dimensional array among MPI processes. 
Exchange halo/boundary values between neighboring processes 
using MPI_Send and MPI_Recv, then perform a stencil computation.
*/
#include <stdio.h>
#include <mpi.h>

#define RADIUS 2

int main(int argc, char *argv[]) {
    int rank, size, data[8] = {10, 20, 30, 40, 50, 60, 70, 80};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 8 / size, loc[n], res[n], l_halo[RADIUS] = {0}, r_halo[RADIUS] = {0};

    MPI_Scatter(data, n, MPI_INT, loc, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank > 0)
        MPI_Sendrecv(loc, RADIUS, MPI_INT, rank - 1, 0, l_halo, RADIUS, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (rank < size - 1)
        MPI_Sendrecv(&loc[n - RADIUS], RADIUS, MPI_INT, rank + 1, 1, r_halo, RADIUS, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < n; i++) {
        int sum = loc[i];
        for (int r = 1; r <= RADIUS; r++) {
            sum += (i - r >= 0) ? loc[i - r] : l_halo[RADIUS - (r - i)];
            sum += (i + r < n) ? loc[i + r] : r_halo[r - (n - i)];
        }
        res[i] = sum;
    }

    int all_res[8];
    MPI_Gather(res, n, MPI_INT, all_res, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < 8; i++) printf("%d ", all_res[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}