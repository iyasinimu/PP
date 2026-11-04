/*
Problem_11: The root process reads configuration parameters 
and broadcasts them to all MPI processes. Each process uses 
the received parameters to perform a computation. 
*/
#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
	int rank, params[3];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		params[0] = 1000;
		params[1] = 50;
		params[2] = 256;
	}

	MPI_Bcast(params, 3, MPI_INT, 0, MPI_COMM_WORLD);
	printf("P%d: Iterations=%d, Threshold=%d, Block=%d\n", rank, params[0], params[1], params[2]);
	MPI_Finalize();
	return 0;
}