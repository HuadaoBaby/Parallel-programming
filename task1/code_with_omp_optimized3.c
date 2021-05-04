#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 20
#define WIN

#ifdef WIN
#include <time.h>
#else
#include <sys/time.h>
#endif

void dgemm_block(double* A, double* B, double* C, int n);
void do_block(double *A, double *B, double *C, int n);
void show_matrix(double* C, int n);
void test(double* A, double* B, double* C, int n);
void get_matrix(double* A, double* B, int n);

void do_block(double *A, double *B, double *C, int n){
    for(int i = 0; i < BLOCKSIZE; i++){
        for(int j = 0; j < BLOCKSIZE; j++){
            double sum = C[i*n+j];
            for(int k = 0; k < BLOCKSIZE; k++ )
                sum += A[i * n + k] * B[k * n + j];
            C[i * n + j] = sum;
        }
    }
}

void dgemm_block(double* A, double* B, double* C, int n){
    #pragma omp parallel for
    for(int j = 0; j < n; j += BLOCKSIZE){
        #pragma omp parallel for
        for(int i = 0; i < n; i += BLOCKSIZE){
            #pragma omp parallel for
            for(int k = 0; k < n; k += BLOCKSIZE){
                do_block(A + i * n + k, B + k * n + j, C + i * n + j, n);
            }
        }
    }
}

void show_matrix(double* C, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%f ", C[i * n + j]);
        }
        printf("\n");
    }
}

void get_matrix(double* A, double* B, int n){
    for(int i = 0; i < n * n; i++){
        A[i] = rand();
        B[i] = rand();
    }
}

#ifdef WIN
void test(double* A, double* B, double* C, int n){
    clock_t start = clock();
    dgemm_block(A, B, C, n);
    clock_t end = clock();
    clock_t time = (end - start) / CLOCKS_PER_SEC;
    printf("The size of matric is %d x %d\n", n, n);
    printf("The time is %d\n", time);
    //printf("The result is \n");
    //show_matrix(C, n);
}
#endif

#ifdef LINUX
void test(double* A, double* B, double* C, int n){
    struct timeval start, end;
    gettimeofday(&start, 0);
    dgemm_block(A, B, C, n);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The size of matric is %d x %d\n", n, n);
    printf("The time is %f\n", time);
    //printf("The result is \n");
    //show_matrix(C, n);
}
#endif

int main(){
    int n = 100;
    while(n <= 2000){
        double* A = (double*)malloc(sizeof(double) * n * n);
        double* B = (double*)malloc(sizeof(double) * n * n);
        double* C = (double*)malloc(sizeof(double) * n * n);
        get_matrix(A, B, n);
        test(A, B, C, n);
        n += 100;
    }
    return 0;
}
