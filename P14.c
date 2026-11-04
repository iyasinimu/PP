/*Each process generates a set of numerical values. 
Use MPI_Reduce to calculate the global sum, maximum, minimum, and average.
*/
#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[]){
	int rank, size, val[3], l_sum = 0, l_max, l_min, g_sum, g_max, g_min;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size != 4) {
		if (rank == 0) printf("Run with 4 processes.\n");
		MPI_Finalize();
		return 0;
	}

	for (int i = 0; i < 3; i++) {
		val[i] = (rank * 30) + (i + 1) * 10;
		l_sum += val[i];
	}
	l_min = val[0];
	l_max = val[2];

	printf("P%d = %d\n", rank, l_sum);

	MPI_Reduce(&l_sum, &g_sum, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
	MPI_Reduce(&l_max, &g_max, 1, MPI_INT, MPI_MAX, 0,MPI_COMM_WORLD);
	MPI_Reduce(&l_min, &g_min, 1, MPI_INT, MPI_MIN, 0,MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Global Sum = %d\nGlobal Maximum = %d\nGlobal Minimum = %d\nAverage = %.2f\n",g_sum, g_max, g_min, (double)g_sum / (size * 3));
	}

	MPI_Finalize();
	return 0;
}