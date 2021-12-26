#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int size, rank1, rank2,sl_size;
	MPI_Status status;
	MPI_Comm intercomm;
	char slave[10] = "./A18_s";
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, 8, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);

	MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
	MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);
	//Display "Slaves rank1 and rank2 are working", instead of the words rank1 and rank2 their values should be displayed.
	cout<< "Slaves "<< rank1 <<" and "<< rank2 <<" are working\n";

	MPI_Recv(&sl_size, 1, MPI_INT, 2, 2, intercomm, &status);
	cout << "The number of running processes:  "<< sl_size << "\n"; 

	MPI_Finalize();
	return 0;
}