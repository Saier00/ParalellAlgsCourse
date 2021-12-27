#include <stdio.h>
#include <iostream>
#include "mpi.h"
using namespace std;
#define BUFSIZE 100

void MPI_File_create_write_random(MPI_Comm comm, const char* filename,MPI_Info info, MPI_File* fh, MPI_Status* status)
{

	MPI_File_open(comm, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, info, fh);
	MPI_File_write(*fh, "StringWithSomeText8124", 30, MPI_CHAR, status);
	MPI_File_close(fh);
}


int main(int argc, char** argv)
{
	int num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[BUFSIZE];

	MPI_File_create_write_random(MPI_COMM_WORLD, "file.txt", MPI_INFO_NULL, &fh, &status);

	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);

	MPI_File_close(&fh);

	//To check file
	getchar();

	MPI_File_delete("file.txt", MPI_INFO_NULL);
	//print the number of read symbols sum from the file
	cout << "Number of read symbols from the file: " << sum;
	MPI_Finalize();
}