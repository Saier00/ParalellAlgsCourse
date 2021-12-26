#include <iostream>
#include "mpi.h"
#include "string"
using namespace std;

int main(int argc, char** argv)
{
	//Color num
	int i = atoi(argv[1]);

	int rank, size, rank1;
	MPI_Init(&argc, &argv);
	MPI_Comm comm_revs;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* colors = new int[3]{ 1,rank % 2,rank % 3 };

	MPI_Comm_split(MPI_COMM_WORLD, colors[i], size - rank, &comm_revs);
	MPI_Comm_rank(comm_revs, &rank1);
	//Display rank and rank1
	cout << "Process rank: " << rank << " | rank1: " << rank1 << "\n";


	MPI_Comm_free(&comm_revs);
	MPI_Finalize();
}