/*
Design a producer–consumer system where one or more MPI processes 
generate data and another process receives and processes the data using 
point-to-point communication.
*/
#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
	int rank, item, data[] = {10, 20, 30, 40, 50, 60, 70, 80};

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < 8; i++)
			MPI_Send(&data[i], 1, MPI_INT, (i % 2) + 1, 0, MPI_COMM_WORLD);

		item = -1;
		MPI_Send(&item, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&item, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	} 
	else{
		while (1) {
			MPI_Recv(&item, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
			if (item == -1) break;
			printf("Consumer P%d consumed %d\n", rank, item);
		}
	}
	MPI_Finalize();
	return 0;
}