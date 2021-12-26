#include <cstdlib>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int mLen = atoi(argv[1]);

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//matrix initialization by each process
	int** a=new int*[mLen];
    int** b=new int*[mLen];
    int** c=new int*[mLen];

    for(int i=0;i<mLen;i++)
    {
        a[i]=new int[mLen];
        b[i]=new int[mLen];
        c[i]=new int[mLen];
    }

	srand(72931);
    int range_min=-10;
    int range_max=10;

    for(int i=0;i<mLen;i++)
    {
        for(int j=0;j<mLen;j++)
        {
			a[i][j] = int(((double)rand() / RAND_MAX) * (range_max - range_min) + range_min);
			b[i][j] = int(((double)rand() / RAND_MAX) * (range_max - range_min) + range_min);
			c[i][j] = 0;
        }
    }

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization

	// start timing for each process
	double stime=MPI_Wtime();

	// matrix multiplication i-k-j
	for (int i = 0; i < mLen; i++)
		for (int k = 0; k < mLen; k++)
			for (int j = 0; j < mLen; j++)
				c[i][j] += a[i][k] * b[k][j];
	
	// end of timing
	double time=MPI_Wtime()-stime;

	// output the execution time of matrix multiplication at each process
	cout << "Process rank: " << rank << " | Time: " << time << "\n";
	MPI_Finalize();
}