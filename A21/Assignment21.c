#include <stdio.h>
#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char** argv)
{
	const int step = 100;
	const int buf_len = 100;

	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	char buf[buf_len +1];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	//change initial destination
	MPI_File_set_view(fh, rank * step, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh, buf, buf_len, MPI_CHAR, MPI_STATUS_IGNORE);
	//To drop last weird symbol in bufer
	buf[buf_len] = '\0';
	printf("process %d, buf=%s\n", rank, buf);
	MPI_File_close(&fh);
	MPI_Finalize();
}