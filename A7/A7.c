#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
	int res;


	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = atoi(argv[1]);
	int* a = new int[n];
	int* b = new int[n];

	if (rank == 0)
	{
		

		for (int i = 0; i < n; i++)
		{
			a[i] = 1;
			b[i] = 1;
		}
	}

	//We could not use it, because Scatter is blocking function
	//MPI_Barrier(MPI_COMM_WORLD);

	int spl = n / size;

	int* sub_a = new int[spl];
	int* sub_b = new int[spl];

	MPI_Scatter(a, spl, MPI_INT, sub_a, spl, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, spl, MPI_INT, sub_b, spl, MPI_INT, 0, MPI_COMM_WORLD);

	int sum = 0;
	for (int i = 0; i < spl; i++)
		sum += sub_a[i] * sub_b[i];

	MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		cout << "Result is " << res;

	MPI_Finalize();

	return 0;
}