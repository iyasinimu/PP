#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 8
int cmp(const void *a, const void *b) {
return (int)a - (int)b;
}int main(int argc, char *argv[]) {
int rank, size, local[2], all[N], data[N] = {45, 12, 78, 3, 56, 23, 89, 10};
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
if (size != 4) {
if (rank == 0) printf("Run with 4 processes\n");
MPI_Finalize();
return 0;
}
MPI_Scatter(data, 2, MPI_INT, local, 2, MPI_INT, 0,
MPI_COMM_WORLD);
qsort(local, 2, sizeof(int), cmp);
MPI_Gather(local, 2, MPI_INT, all, 2, MPI_INT, 0,
MPI_COMM_WORLD);
if (rank == 0) {
qsort(all, N, sizeof(int), cmp);
printf("Sorted array: ");
for (int i = 0; i < N; i++) printf("%d ", all[i]);
printf("\n");
}
MPI_Finalize();
return 0;
}