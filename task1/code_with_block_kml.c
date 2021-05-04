//块运算 康梦露

#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "omp.h"
int BLOCKSIZE = 20;

void create_matrix(double* A,double* B,int n);
void general_multiple(double* A, double* B, double* C, int n);

void create_matrix(double* A, double* B, int n){
    for(int i = 0; i < n * n; i++){
        A[i] = rand();
        B[i] = rand();
    }
}

void do_block (int n, double *A, double *B, double *C){
    for(int i = 0; i < BLOCKSIZE; i++){
        for(int j = 0; j < BLOCKSIZE; j++){
            double cij = C[i*n+j];
            for(int k = 0; k < BLOCKSIZE; k++ )
                cij +=A[i*n+k] * B[k*n + j];
            C[i*n+j] = cij;
        }
    }
}

void dgemm_block (int n, double* A, double* B, double* C)
{
    #pragma omp parallel for
    for ( int sj = 0; sj < n; sj += BLOCKSIZE ){
        #pragma omp parallel for
        for ( int si = 0; si < n; si += BLOCKSIZE ){
            #pragma omp parallel for
            for ( int sk = 0; sk < n; sk += BLOCKSIZE ){
                do_block(n, A+si*n+sk, B+sk*n+sj, C+si*n+sj);
            }
        }
}
}



int main(){
    int n = 100;
    while(n <= 2000){
        double* A = (double*)malloc(sizeof(double) * n * n);
        double* B = (double*)malloc(sizeof(double) * n * n);
        double* C = (double*)malloc(sizeof(double) * n * n);
        create_matrix(A, B, n);
        
        struct timeval start, end;
        gettimeofday(&start, 0);
        dgemm_block(n, A, B, C);
        gettimeofday(&end, 0);
        double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        time /= 1000;
        //printf("The size of matric is %d x %d\n", n, n);
        //printf("The time is %f\n", time);
        printf("%f\n", time);
        n += 100;
        }
    return 0;
}