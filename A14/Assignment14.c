#include <stdio.h>
#include "mpi.h"
#define n 1000

//Find max element between elements of process in one position
void max(void* a, void* b, int* l, MPI_Datatype* type) {
	for (int i = 0; i < *l; i++) 
	{
		if (((int*)a)[i] > ((int*)b)[i])
			((int*)b)[i] = ((int*)a)[i];
	}
}

int main(int argc, char** argv)
{
	int rank, size, i;
	int a[n];
	int b[n];

	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (i = 0; i < n; i++) a[i] = i + rank + 1;

	printf("process %d a[0] = %d\n", rank, a[0]);
	MPI_Op_create(&max, 1, &op);
	MPI_Reduce(a, b, n, MPI_INT, op, 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);
	if (rank == 0) printf("Max a[0] by custom op= %d\n", b[0]);

	MPI_Reduce(a, b, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0) printf("max a[0] by MPI_MAX= %d\n", b[0]);
	MPI_Finalize();
}