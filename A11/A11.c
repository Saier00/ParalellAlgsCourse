#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, prev, next;
	int buf[2];
	MPI_Init(&argc, &argv);
	MPI_Status stats;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	prev = rank - 1;
	next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;

	MPI_Sendrecv(&rank, 1, MPI_INT, next, 0, &buf[0], 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &stats);
	MPI_Sendrecv(&rank, 1, MPI_INT, prev, 0, &buf[1], 1, MPI_INT, next, 0, MPI_COMM_WORLD, &stats);
	
	//Your code here.
	//Here you need to display the number of the current process, and what it receives from the previous and next processes.
	cout<<"Process rank: "<<rank<<" | Previous: "<<buf[0]<<" | Next: "<<buf[1]<<"\n";
	MPI_Finalize();
}