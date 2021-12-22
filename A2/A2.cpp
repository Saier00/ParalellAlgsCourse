#include <stdio.h>
#include <iostream>
#include <omp.h>

using namespace std;

void zero_init_matrix(int** matrix, long int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0 ;
}


int main(int argc, char* argv[])
{
    int mLen=atoi(argv[1]);

    int** a=new int*[mLen];
    int** b=new int*[mLen];
    int** c=new int*[mLen];

    bool isDebug=false;
    if(mLen==5)
        isDebug=true;

    for(int i=0;i<mLen;i++)
    {
        a[i]=new int[mLen];
        b[i]=new int[mLen];
        c[i]=new int[mLen];
    }

    //for a and b matrices all elements are random values
    //to avoid overflow we should use small values.
    //Also, we could add negative signed values,
    //so mean value will be 0 for matrix c
    srand(72931);
    int range_min=-10;
    int range_max=10;

    for(int i=0;i<mLen;i++)
    {
        for(int j=0;j<mLen;j++)
        {
            a[i][j]=((double)rand() / RAND_MAX) * (range_max - range_min) + range_min;
            b[i][j]=((double)rand() / RAND_MAX) * (range_max - range_min) + range_min;
            //c is 0 just to avoid additional ifs
            c[i][j]=0;

        }
    }

    //print 5*5 matrices a and b
    if(isDebug)
    {   
        cout<<"Matrix A:\n";
        for(int i=0;i<mLen;i++)
        {
            for(int j=0;j<mLen;j++)
            {
                cout << a[i][j] << " ";
            }

            cout << "\n";
        }

        cout<<"\nMatrix B:\n";
        for(int i=0;i<mLen;i++)
        {
            for(int j=0;j<mLen;j++)
            {
                cout << b[i][j] << " ";
            }

            cout << "\n";
        }
    }


    //to simplify program, we will just use the same array with times for every combination i-j-k
    double* times = new double[10];

    //i-j-k
    //for number of threads from 1 to 10
    for (int threads=1; threads<=10;threads++)
    {

        times[threads-1]=-omp_get_wtime();

        #pragma omp parallel for shared(a,b,c) num_threads(threads)
        for (int i = 0; i < mLen; i++)
            for (int j = 0; j < mLen; j++)
                for (int k = 0; k < mLen; k++)
                    c[i][j] += a[i][k] * b[k][j];

        times[threads-1]+=omp_get_wtime();
    }

    cout << "\nfor i-j-k multiplication\n";
    cout << "Number of threads |     Time    | Efficiency\n";

    for(int i=0;i<10;i++)
    {
        printf("%17d | %0.5e | %1.3f\n",i+1,times[i],times[0]/times[i]);
    }
    
    if(isDebug)
    {   
        cout<<"\nMatrix C i-j-k:\n";
        for(int i=0;i<mLen;i++)
        {
            for(int j=0;j<mLen;j++)
            {
                cout << b[i][j] << " ";
            }

            cout << "\n";
        }
    }

    zero_init_matrix(c,mLen);

    //j-k-i
    //for number of threads from 1 to 10
    for (int threads=1; threads<=10;threads++)
    {

        times[threads-1]=-omp_get_wtime();

        #pragma omp parallel for shared(a,b,c) num_threads(threads)
        for (int j = 0; j < mLen; j++)
            for (int k = 0; k < mLen; k++)
                for (int i = 0; i < mLen; i++)
                    c[i][j] += a[i][k] * b[k][j];

        times[threads-1]+=omp_get_wtime();
    }

    cout << "\nfor j-k-i multiplication\n";
    cout << "Number of threads |     Time    | Efficiency\n";

    for(int i=0;i<10;i++)
    {
        printf("%17d | %0.5e | %1.3f\n",i+1,times[i],times[0]/times[i]);
    }


    if(isDebug)
    {   
        cout<<"\nMatrix C j-k-i:\n";
        for(int i=0;i<mLen;i++)
        {
            for(int j=0;j<mLen;j++)
            {
                cout << b[i][j] << " ";
            }

            cout << "\n";
        }
    }

    zero_init_matrix(c,mLen);

    //i-k-j
    //for number of threads from 1 to 10
    for (int threads=1; threads<=10;threads++)
    {

        times[threads-1]=-omp_get_wtime();

        #pragma omp parallel for shared(a,b,c) num_threads(threads)
        for (int i = 0; i < mLen; i++)
            for (int k = 0; k < mLen; k++)
                for (int j = 0; j < mLen; j++)
                    c[i][j] += a[i][k] * b[k][j];

        times[threads-1]+=omp_get_wtime();
    }

    cout << "\nfor i-k-j multiplication\n";
    cout << "Number of threads |     Time    | Efficiency\n";

    for(int i=0;i<10;i++)
    {
        printf("%17d | %0.5e | %1.3f\n",i+1,times[i],times[0]/times[i]);
    }



    if(isDebug)
    {   
        cout<<"\nMatrix C i-k-j:\n";
        for(int i=0;i<mLen;i++)
        {
            for(int j=0;j<mLen;j++)
            {
                cout << b[i][j] << " ";
            }

            cout << "\n";
        }
    }
    return 0;
}