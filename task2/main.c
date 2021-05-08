//task2 归并排序和快速排序 刘泽坤

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

#define WIN

#ifdef WIN
#include <time.h>
#else
#include <sys/time.h>
#endif

void quicksort(int* A, int start, int end);
void quicksort_omp(int* A, int start, int end);
void mergesort(int* A, int* temp, int start, int end);
void mergesort_omp(int* A, int* temp, int start, int end);
void show_vector(int* A, int n);
void test(int* A, int n);
void get_vector(int* A, int n, int mode);

void show_vector(int* A, int n){
    for(int i = 0; i < n; i++){
        printf("%d ", A[i]);
    }
}

void get_vector(int* A, int n, int mode){
    if(mode == 0){
        A[0] = rand();
        for(int i = 1; i < n; i++){
            A[i] = rand()%5 + A[i - 1];
        }
    }
    else if(mode == 1){
        for(int i = 0; i < n; i++){
            A[i] = rand()%5 + A[i - 1];
        }
    }
    else{
        A[0] = rand();
        for(int i = 1; i < n; i++){
            A[i] = rand()%5 - A[i - 1];
        }
    }
    printf("Create vector, successfully, mode is %d\n", mode);
}

void quicksort(int* A, int start, int end){
    if(start >= end) return;
    int partition = A[start];
    int i = start, j = end;
    while(i < j){
        while(i < j && A[j] >= partition) j--;
        if(i < j){
            A[i] = A[j];
            i++;
        }
        while(i < j && A[j] < partition) i++;
        if(i < j){
            A[j] = A[i];
            j--;
        }
    }
    A[i] = partition;
    quicksort(A, start, i-1);
    quicksort(A, i+1, end);
}

void quicksort_omp(int* A, int start, int end){
    if(start >= end) return;
    int partition = A[start];
    int i = start, j = end;
    while(i < j){
        while(i < j && A[j] >= partition) j--;
        if(i < j){
            A[i] = A[j];
            i++;
        }
        while(i < j && A[j] < partition) i++;
        if(i < j){
            A[j] = A[i];
            j--;
        }
    }
    A[i] = partition;
    #pragma omp sections
    {
        #pragma omp section{
        quicksort_omp(A, start, i-1);
        #pragma omp section{
        quicksort_omp(A, i+1, end);
    }
}

void mergesort(int* A, int* temp, int start, int end){
    if (start >= end) return;
    int mid = (end - start) / 2 + start;
    int start1 = start;
    int end1 = mid;
    int start2 = mid + 1;
    int end2 = end;
    mergesort(A, temp, start1, end1);
    mergesort(A, temp, start2, end2);
    int pointer = start;
    while(start1 <= end1 && start2 <= end2){
        if (A[start1] < A[start2]){
            temp[pointer] = A[start1];
            start1++;
        }
        else{
            temp[pointer] = A[start2];
            start2++;
        }
        pointer++;
    }
    for(int i = start1; i <= end1; i++){
        temp[pointer] = A[i];
        pointer++;
    }
    for(int i = start2; i <= end2; i++){
        temp[pointer] = A[i];
        pointer++;
    }
    for(int i = start; i <= end; i++){
        A[i] = temp[i];
    }
}

void mergesort_omp(int* A, int* temp, int start, int end){
    if (start >= end) return;
    int mid = (end - start) / 2 + start;
    int start1 = start;
    int end1 = mid;
    int start2 = mid + 1;
    int end2 = end;
    if(end - start < 1500){
        mergesort(A, temp, start1, end1);
        mergesort(A, temp, start2, end2);
    }
    else{
        #pragma omp task
        mergesort_omp(A, temp, start1, end1);
        #pragma omp task
        mergesort_omp(A, temp, start2, end2);
        #pragma omp taskwait
    }
    int pointer = start;
    while(start1 <= end1 && start2 <= end2){
        if (A[start1] < A[start2]){
            temp[pointer] = A[start1];
            start1++;
        }
        else{
            temp[pointer] = A[start2];
            start2++;
        }
        pointer++;
    }
    for(int i = start1; i <= end1; i++){
        temp[pointer] = A[i];
        pointer++;
    }
    for(int i = start2; i <= end2; i++){
        temp[pointer] = A[i];
        pointer++;
    }
    for(int i = start; i <= end; i++){
        A[i] = temp[i];
    }
}

#ifdef WIN
void test(int* A, int n){
    int* temp = (int*)malloc(sizeof(int) * n);
    printf("The size of vector is %d\n", n);
    clock_t start1 = clock();
    quicksort(A, 0, n-1);
    clock_t end1 = clock();
    clock_t time1 = (end1 - start1) / CLOCKS_PER_SEC;
    printf("The quicksort time is %d\n", time1);
    
    clock_t start2 = clock();
    quicksort_omp(A, 0, n-1);
    clock_t end2 = clock();
    clock_t time2 = (end2 - start2) / CLOCKS_PER_SEC;
    printf("The quicksort_omp time is %d\n", time2);
    
    clock_t start3 = clock();
    mergesort(A, temp, 0, n-1);
    clock_t end3 = clock();
    clock_t time3 = (end3 - start3) / CLOCKS_PER_SEC;
    printf("The mergesort time is %d\n", time3);

    clock_t start4 = clock();
    mergesort_omp(A, temp, 0, n-1);
    clock_t end4 = clock();
    clock_t time4 = (end4 - start4) / CLOCKS_PER_SEC;
    printf("The mergesort_omp time is %d\n", time4);
    //printf("The result is \n");
    //show_matrix(C, n);
}
#endif

#ifdef LINUX
void test(int*A int n){
    int* temp = (int*)malloc(sizeof(int) * n);
    printf("The size of vector is %d\n", n);
    struct timeval start, end;

    gettimeofday(&start, 0);
    quicksort(A, 0, n-1);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The quicksort time is %f\n", time);

    gettimeofday(&start, 0);
    quicksort_omp(A, 0, n-1);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The quicksort_omp time is %f\n", time);

    gettimeofday(&start, 0);
    mergesort(A, temp, 0, n-1);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The mergesort time is %f\n", time);

    gettimeofday(&start, 0);
    mergesort_omp(A, temp, 0, n-1);
    gettimeofday(&end, 0);
    double time = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time /= 1000;
    printf("The mergesort_omp time is %f\n", time);

    //printf("The result is \n");
    //show_matrix(C, n);
}
#endif

int main(){
    for(int n = 10; n <= 100000000; n *= 10){
        int* A = (int*)malloc(sizeof(int) * n);
        for(int i = 0; i < 8; i++){
            get_vector(A, n, 0);
            test(A, n);
        }
        for(int i = 0; i < 8; i++){
            get_vector(A, n, 1);
            test(A, n);
        }
        for(int i = 0; i < 8; i++){
            get_vector(A, n, 2);
            test(A, n);
        }
    }
    return 0;
}
