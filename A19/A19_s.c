#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char** argv)
{
	int rcvd;
	int snd = 25;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;

	MPI_Open_port(MPI_INFO_NULL, port_name);
	//Server shows this string only 
	//with cout and only with endl.
	//I have no idea why
	cout << "Portname: " << port_name << endl;
	
	cout << "Waiting for the client ...\n";

	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);

	cout << "Client connected\n";

	MPI_Send(&snd, 1, MPI_INT, 0, 0, intercomm);
	cout << "Server sent value: " << snd << "\n";

	MPI_Recv(&rcvd, 1, MPI_INT, 0, 0, intercomm, &status);
	cout << "Server got value: " << rcvd << "\n";
	

	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);

	MPI_Finalize();
	return 0;
}