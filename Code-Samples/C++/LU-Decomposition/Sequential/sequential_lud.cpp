#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>    
using namespace std;
float** InitializeMatrix(int n, float value)
{
	// allocate square 2d matrix
	float **x = new float*[n];
	x[0] = new float[n*n];
	for (int i = 1; i < n; i++)	x[i] = x[i-1] + n;
	for (int i = 0 ; i < n ; i++)
	{
		for (int j = 0 ; j < n ; j++)
		{
            if (value == 1)  // generate input matrices (a and b)
                x[i][j] = (float)(rand()%10 + 1);
            else
                x[i][j] = 0;  // initializing resulting matrix
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
		for (int j = 0 ; j < n ; j++) printf("%.2f\t", a[j][i]);
		cout << endl;
	}
}
void LUDecomposition(float** a, float** l, float** u, int n)
{
	int i = 0, j = 0, k = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (j < i) u[j][i] = 0;
			else {
				u[j][i] = a[j][i];
				for (k = 0; k < i; k++) {
					u[j][i] = u[j][i] - u[j][k] * l[k][i];
				}
			}
		}
		for (j = 0; j < n; j++) {
			if (j < i) l[i][j] = 0;
			else if (j == i) l[i][j] = 1;
			else {
				l[i][j] = a[i][j] / u[i][i];
				for (k = 0; k < i; k++) {
					l[i][j] = l[i][j] - ((u[i][k] * l[k][j]) / u[i][i]);
				}
			}
		}
	}
}
int main(int argc, char** argv)
{
	float **a, **l, **u, **c;
	double runtime;
	srand(1);
	int n = atoi(argv[1]);
	a = InitializeMatrix(n,1);
	l = InitializeMatrix(n,0);
	u = InitializeMatrix(n,0);
	runtime = clock()/(float)CLOCKS_PER_SEC;
	LUDecomposition(a,l,u,n);
	runtime = (clock()/(float)CLOCKS_PER_SEC ) - runtime;
	if(n < 10){
		cout << "A Matrix: " << endl;
	    	PrintMatrix(a, n);
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
	DeleteMatrix(c,n);
	return 0;
}
