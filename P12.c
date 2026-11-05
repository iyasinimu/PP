/*
Problem_12: Use MPI_Scatter to distribute a large vector among processes. 
Each process calculates the square or cube of its elements, and 
MPI_Gather collects the results at the root process. 
*/
#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[]) {
int rank, local[2], res[8], data[] = {1, 2, 3, 4, 5, 6, 7, 8};MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Scatter(data, 2, MPI_INT, local, 2, MPI_INT, 0,
MPI_COMM_WORLD);
local[0] *= local[0];
local[1] *= local[1];
MPI_Gather(local, 2, MPI_INT, res, 2, MPI_INT, 0,
MPI_COMM_WORLD);
if (rank == 0) {
printf("Squared vector: ");
for (int i = 0; i < 8; i++) printf("%d ", res[i]);
printf("\n");
}
MPI_Finalize();
return 0;
}