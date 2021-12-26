#include <iostream>
#include <mpi.h>
using namespace std;

bool IsPowerOfTwo(int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

int main(int argc, char* argv[]) {
	int sum_reduce = 0;
	int sum_db = 0;
	double time_reduce, time_db;

	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = atoi(argv[1]);
	int* a = new int[n];

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			a[i] = 1;
		}
	}

	int spl = n / size;

#pragma region reduce
	//Reduce from A7.c:
	int* sub_a = new int[spl];

	//We will include scatter operation in both methods too
	double stime = MPI_Wtime();

	MPI_Scatter(a, spl, MPI_INT, sub_a, spl, MPI_INT, 0, MPI_COMM_WORLD);


	int sum = 0;
	for (int i = 0; i < spl; i++)
		sum += sub_a[i];

	MPI_Reduce(&sum, &sum_reduce, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	time_reduce = MPI_Wtime() - stime;

#pragma endregion reduce

#pragma region db
	sub_a = new int[spl];

	stime = MPI_Wtime();

	MPI_Scatter(a, spl, MPI_INT, sub_a, spl, MPI_INT, 0, MPI_COMM_WORLD);

	sum = 0;
	for (int i = 0; i < spl; i++)
		sum += sub_a[i];

	bool isSended = false;
	//We will gather sums using doubling scheme to even processes (for every iteration)
	for (int gatherer_rank = 2; gatherer_rank <= size; gatherer_rank *= 2)
	{
		if (rank % gatherer_rank == 0)
		{
			//If we have the number of processes isn't power of two
			//we will gather them later
			if (rank + gatherer_rank / 2 < size)
			{
				//GATHER
				int rcv;
				//Receive message from one odd process
				MPI_Recv(&rcv, 1, MPI_INT, rank + gatherer_rank / 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				sum += rcv;
			}
		}
		else
		{
			//SEND
			MPI_Send(&sum, 1, MPI_INT, rank - gatherer_rank / 2, 0, MPI_COMM_WORLD);
			isSended = true;
			break;
		}
	}

	/* code for the num of processes is not power of two
	if (!IsPowerOfTwo(size))
	{
		if (rank == 0)
		{	
			cout << "ANY " << (status.MPI_SOURCE == MPI_ANY_SOURCE) << "\n";
			cout << "stat " << status.MPI_SOURCE << "\n";
			cout << "iss " << isSended << "\n";
			//At least one msg
			do {
				int rcv;
				MPI_Recv(&rcv, 1, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &status);
				sum += rcv;

				MPI_Probe(MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &status);
				cout << "HERE!\n";
			} while (status.MPI_SOURCE == MPI_ANY_SOURCE);
		}
		else if (!isSended)
		{
			MPI_Send(&sum, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
		}
	}
	*/
	if (rank == 0)
	{
		sum_db = sum;
		time_db = MPI_Wtime() - stime;
	}

	
#pragma region db

	if (rank == 0)
	{
		cout << "Sum for reducton: " << sum_reduce << "\n";
		cout << "Time for reduction: " << time_reduce << "\n";

		cout << "Sum for doubling: " << sum_db << "\n";
		cout << "Time for doubling: " << time_db << "\n";
	}

	MPI_Finalize();

	return 0;
}