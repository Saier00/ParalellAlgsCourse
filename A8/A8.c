<iostream>
#include <mpi.h>
#include <math.h>
using namespace std;

int main(int argc, char* argv[]) {

	srand(9591);

	//num of elements in message
	int n = atoi(argv[1]);
	//num of send-receive iters
	int iters = 10;

	//we will send part of the same message every time
	int* mes = new int[n];
	for (int i = 0; i < n; i++)
		mes[i] = rand();

	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double stime;
	double* times = new double[int(log10(n)) + 2];
	int* elements = new int[int(log10(n)) + 2];
	int* dump = new int[n];

	//We will find latency, using the same loop
	//for 0 elements
	int i = 0;
	while (i <= n)
	{
		stime = MPI_Wtime();

		for (int it = 0; it < iters; it++)
		{
			if (rank == 0)
			{
				MPI_Send(mes, i, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Recv(dump, i, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			}
			else if (rank == 1)
			{
				MPI_Recv(dump, i, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				MPI_Send(mes, i, MPI_INT, 0, 1, MPI_COMM_WORLD);
			}
		}

		if (i == 0)
		{
			times[0] = MPI_Wtime() - stime;
			elements[0] = 0;
			i = 1;
		}
		else
		{
			times[int(log10(i)) +1] = MPI_Wtime() - stime;
			elements[int(log10(i)) +1] = i;
			i *= 10;
		}
	}

	if (rank == 0)
	{
		cout << "\nNumber of elements | Length in bytes |     Time    |  Bandwidth  | Latency\n";

		for (int i = 0; i < int(log10(n)) + 2; i++)
		{
			if (elements[i] == 0)
				printf("%18d | %15d | %0.5e |             | %f\n", elements[i], int(elements[i] * sizeof(int)), times[i], times[i] / 2.0 * iters);
			else
				printf("%18d | %15d | %0.5e | %0.5e | \n", elements[i], int(elements[i] * sizeof(int)), times[i], 2.0 * (elements[i] * sizeof(int)) * iters / times[i]);
		}
	}


	MPI_Finalize();

	return 0;
}