#include <stdio.h>
#include <iostream>
#include <omp.h>

using namespace std;
int main(int argc, char* argv[])
{
    int vLen=atoi(argv[1]);
    //int nums from 0 to RAND_MAX
    int* v=new int[vLen];
    srand(634621);
    for(int i=0;i<vLen;i++)
    {
        v[i]=rand();
        /*for debug
        if(i<10)
        {
            cout<<v[i]<<" ";
        }
        */
    }

    double* times = new double[10];
    int* maxValues=new int[10];
    //for number of threads from 1 to 10
    for (int threads=1; threads<=10;threads++)
    {

        times[threads-1]=-omp_get_wtime();

        int max_val = 0;

        #pragma omp parallel shared(v) reduction (max:max_val) num_threads(threads)
        #pragma omp for
        for (int i = 0; i < vLen; i++)
        {
            max_val = max_val < v[i] ? v[i] : max_val;
        }

        times[threads-1]+=omp_get_wtime();
        maxValues[threads-1]=max_val;
    }

    cout << "\nNumber of threads | Max Value | Time\n";

    for(int i=0;i<10;i++)
    {
        printf("%17d | %9d | %0.5e\n",i+1,maxValues[i],times[i]);
    }
    return 0;
}