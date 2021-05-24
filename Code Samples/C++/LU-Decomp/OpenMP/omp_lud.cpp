#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>    
#include <omp.h>
using namespace std;
bool GetUserInput(int argc, char *argv[],int& n,int& numThreads,bool& isPrint)
{
	bool isOK = true;

	if(argc < 3) 
	{
		cout << "Arguments:<X> <Y> [<Z>]" << endl;
		cout << "X : Matrix size [X x X]" << endl;
		cout << "Y : Number of threads" << endl;
		cout << "Z = 1: print the input/output matrix if X < 10" << endl;
		cout << "Z <> 1 or missing: does not print the input/output matrix" << endl;
		isOK = false;
	}
	else 
	{
		//get matrix size
		n = atoi(argv[1]);
		if (n <=0) 
		{
			cout << "Matrix size must be larger than 0" <<endl;
			isOK = false;
		}

		//get number of threads
		numThreads = atoi(argv[2]);
		if (numThreads <= 0)
		{	cout << "Number of threads must be larger than 0" <<endl;
			isOK = false;
		}

		//is print the input/output matrix
		if (argc >=4) isPrint = (atoi(argv[3])==1 && n <=9)?1:0;
		else isPrint = 0;
	}
	return isOK;
}
float** InitializeMatrix(int n, float value)
{
	// allocate square 2d matrix
	float **x = new float*[n];
	x[0] = new float[n*n];
	for (int i = 1; i < n; i++) x[i] = x[i-1] + n;
	for (int i = 0 ; i < n ; i++)
	{
		for (int j = 0 ; j < n ; j++)
		{
		    if (value == 1) x[i][j] = (float)(rand()%10 + 1);
		    else x[i][j] = 0;  // initializing resulting matrix
		}
	}
	return x;
}
//------------------------------------------------------------------
//delete matrix matrix a[n x n]
//------------------------------------------------------------------
void DeleteMatrix(float **a,int n)
{
	delete[] a[0];
	delete[] a; 
}
void PrintMatrix(float **a, int n) 
{
	for (int i = 0 ; i < n ; i++)
	{
		cout<< "Row " << (i+1) << ":\t" ;
		for (int j = 0 ; j < n ; j++)
		{
			printf("%.2f\t", a[i][j]);
		}
		cout << endl;
	}
}
void LUDecomposition(float** a, float** l, float** u, int n)
{
//initialize a simple lock for parallel region
	omp_lock_t lock;
	omp_init_lock(&lock);
	#pragma omp parallel shared(a,l,u)
	{
		for (int i = 0; i < n; i++)
		{
			#pragma omp for schedule(static)
			for (int j = 0; j < n; j++)
			{
				if (j < i) l[j][i] = 0;
				else{	
					l[j][i] = a[j][i];
					for (int k = 0; k < i; k++) l[j][i] = l[j][i] - l[j][k] * u[k][i];
				}
			}
			#pragma omp for schedule(static)
			for (int j = 0; j < n; j++)
			{
				if (j < i) u[i][j] = 0;
				else if (j == i) u[i][j] = 1;
				else{
					u[i][j] = a[i][j] / l[i][i];
					for (int k = 0; k < i; k++) u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
				}
			}
		}
	}
}
int main(int argc, char** argv)
{
	float **a, **l, **u;
	int n;
	int numThreads;
	bool isPrintMatrix;
	double runtime;
	srand(1);
	if (GetUserInput(argc,argv,n,numThreads,isPrintMatrix)==false) return 1;
	//specify number of threads created in parallel region
	omp_set_num_threads(numThreads);
	a = InitializeMatrix(n,1);
	if(n < 10){
		cout << "A Matrix: " << endl;
		PrintMatrix(a, n);
	}
	l = InitializeMatrix(n,0);
	u = InitializeMatrix(n,0);
	runtime = omp_get_wtime();
	LUDecomposition(a,l,u,n);
	runtime = omp_get_wtime() - runtime;
	if(n < 10){
		cout << "L Matrix: " << endl;
		PrintMatrix(l, n);
		cout << "U Matrix: " << endl;
		PrintMatrix(u, n);
	}
	cout<< "LU Decomposition runs in "	<< setiosflags(ios::fixed) 
						<< setprecision(2)  
						<< runtime << " seconds \n";
	DeleteMatrix(a,n);
	DeleteMatrix(u,n);
	DeleteMatrix(l,n);
	return 0;
}
