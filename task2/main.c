//task2 归并排序和快速排序 刘泽坤

#include <stdio.h>
#include <stdlib.h>

#define WIN

#ifdef WIN
#include <time.h>
#else
#include <sys/time.h>
#endif

void quicksort(int* A, int n);
void mergesort(int* A, int n);
void show_vector(int* A, int n);
void test(int* A, int n);
void get_vector(int* A, int n);

void show_vector(int* A, int n){
    for(int i = 0; i < n; i++){
        printf("%d ", A[i]);
    }
}

void get_vector(int* A, int n){
    for(int i = 0; i < n; i++){
        A[i] = rand();
    }
}

void quicksort(int* A, int n){

}

void mergesort(int* A, int n){

}

#ifdef WIN
void test(int* A, int n){
    printf("The size of vector is %d\n", n);
    clock_t start = clock();
    quicksort(A, n);
    clock_t end = clock();
    clock_t time = (end - start) / CLOCKS_PER_SEC;
    printf("The quicksort time is %d\n", time);
    clock_t start = clock();
    mergesort(A, n);
    clock_t end = clock();
    clock_t time = (end - start) / CLOCKS_PER_SEC;
    printf("The mergesort time is %d\n", time);
    //printf("The result is \n");
    //show_matrix(C, n);
}
#endif

#ifdef LINUX
void test(int*A int n){
    printf("The size of vector is %d\n", n);
    struct timeval start, end;
    gettimeofday(&start, 0);
    quicksort(A, n);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The quicksort time is %f\n", time);
    gettimeofday(&start, 0);
    mergesort(A, n);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The mergesort time is %f\n", time);
    //printf("The result is \n");
    //show_matrix(C, n);
}
#endif

int main(){
    int n = 10;
    while(n <= 100000000){
        int* A = (int*)malloc(sizeof(int) * n);
        get_vector(A, n);
        test(A, n);
        n *= 10;
    }
    return 0;
}
