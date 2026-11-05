#include <stdio.h>
#include <mpi.h>

double f(double x) { // Function to integrate: f(x) = x^2
    return x * x; 
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 10.0;[cite: 1]
    int n = 1000000;          // Number of trapezoids
    double h = (b - a) / n;  [cite: 1]
    double local_sum = 0.0, global_sum = 0.0;

    if (rank == 0) {
        printf("Number of Processes = %d\n\n", size);
    }

    /* Distribute loop range [1, n-1] across processes */
    int total_iterations = n - 1;
    int local_n = total_iterations / size;
    int start = 1 + rank * local_n;
    int end = (rank == size - 1) ? n : start + local_n;

    /* Parallel computation of interior points */
    for (int i = start; i < end; i++) {
        // printf("Process %d computes i = %d\n", rank, i); // Optional execution log
        double x = a + i * h;[cite: 1]
        local_sum += f(x);   [cite: 1]
    }

    /* Combine local_sum from all ranks into global_sum at rank 0 */
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Rank 0 calculates final trapezoidal formula */
    if (rank == 0) {
        double integral = (h / 2.0) * (f(a) + 2.0 * global_sum + f(b)); // Apply Trapezoidal Rule[cite: 1]
        printf("Approximate Integral = %.12lf\n", integral);[cite: 1]
    }

    MPI_Finalize();
    return 0;
}