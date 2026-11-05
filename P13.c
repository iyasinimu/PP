/*Distribute matrix rows among processes when the number of rows is not evenly 
divisible by the number of processes. Collect the processed rows using MPI_Gatherv.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 5
#define COLS 3

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int full[ROWS * COLS], result[ROWS * COLS];
    int sendcounts[size], displs[size];

    /* Calculate row distribution (remainder goes to initial ranks) */
    int base_rows = ROWS / size, rem = ROWS % size;
    int local_rows = base_rows + (rank < rem ? 1 : 0);
    int local[local_rows * COLS];

    /* Set up counts and displacements array on rank 0 */
    if (rank == 0) {
        int matrix[ROWS][COLS] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15}};
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++) full[i * COLS + j] = matrix[i][j];

        int offset = 0;
        for (int i = 0; i < size; i++) {
            int r = base_rows + (i < rem ? 1 : 0);
            sendcounts[i] = r * COLS;
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    /* Distribute variable row chunks */
    MPI_Scatterv(full, sendcounts, displs, MPI_INT, local, local_rows * COLS, MPI_INT, 0, MPI_COMM_WORLD);

    /* Process local rows: multiply elements by 2 */
    printf("P%d:\n", rank);
    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < COLS; j++) {
            local[i * COLS + j] *= 2;
            printf("%d ", local[i * COLS + j]);
        }
        printf("\n");
    }

    /* Gather variable row chunks back to rank 0 */
    MPI_Gatherv(local, local_rows * COLS, MPI_INT, result, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final matrix:\n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) printf("%d ", result[i * COLS + j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}