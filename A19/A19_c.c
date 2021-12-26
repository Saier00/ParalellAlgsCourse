#include <iostream>
#include <string>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int snd=42;
	int rcvd;

	MPI_Init(&argc, &argv);

	char port_name[MPI_MAX_PORT_NAME];

	MPI_Status status;
	MPI_Comm intercomm;

	strcpy_s(port_name, argv[1]);
	cout << "Attempt to connect\n";

	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	cout << "Server connection\n";
	
	MPI_Send(&snd, 1, MPI_INT, 0, 0, intercomm);
	MPI_Recv(&rcvd, 1, MPI_INT, 0, 0, intercomm, &status);

	cout << "Client sent value: " << snd << "\n";
	cout << "Client got value: " << rcvd << "\n";

	MPI_Finalize();
	return 0;
}