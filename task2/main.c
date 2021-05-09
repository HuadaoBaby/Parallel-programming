//task2 归并排序和快速排序 刘泽坤

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "omp.h"

void my_sort(int* array, int n);
void my_sort_omp(int* array, int n);
void quicksort_omp(int *array, int start, int end);
void quicksort(int *array, int start, int end);
int partition(int *array, int start, int end);
void swap(int *a, int *b);
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
    srand(time(NULL));
    if(mode == 0){
        A[0] = rand();
        for(int i = 1; i < n; i++){
            A[i] = rand()%n + A[i - 1];
        }
    }
    else if(mode == 1){
        for(int i = 0; i < n; i++){
            A[i] = rand()%n + A[i - 1];
        }
    }
    else{
        A[0] = rand();
        for(int i = 1; i < n; i++){
            A[i] = rand()%n - A[i - 1];
        }
    }
    printf("Create vector, successfully, mode is %d\n", mode);
}

void swap(int *a, int *b){
    int temp = *b;
    *b = *a;
    *a = temp;
}

int partition(int *array, int start, int end){
    int pivot = array[end];
    int i = start;
    for(int j = start; j <= end; j++){
        if(array[j] < pivot){
            swap(&array[i], &array[j]);
            i++;
        }
    }
    swap(&array[i], &array[end]);
    return i;
}

void quicksort(int *array, int start, int end){
    if (start >= end) return;
    int index = partition(array, start, end);
    quicksort(array, start, index - 1);
    quicksort(array, index + 1, end);
}

void quicksort_omp(int *array, int start, int end){
    if (start >= end) return;
    int index = partition(array, start, end);
    #pragma omp parallel sections shared(array)
    {
        #pragma omp section
        {
            quicksort(array, start, index - 1);
            //printf("running 1\n");
        }
        #pragma omp section
        {
            //printf("running 2\n");
            quicksort(array, index + 1, end);
        }
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
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mergesort(A, temp, start1, end1);
        }
        #pragma omp section
        {
            mergesort(A, temp, start2, end2);
        }
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

void my_sort(int* array, int n){
    int* temp = (int*)malloc(sizeof(int) * n);
    memcpy(temp, array, sizeof(int) * n);
    for (int i = 0; i < n; i++){
        int count = 0;
        for (int j = 0; j < n; j++){
            if (temp[i] > temp[j])
                count++;
        }
        array[count] = temp[i];
    }
}


void my_sort_omp(int* array, int n){
    int* temp = (int*)malloc(sizeof(int) * n);
    memcpy(temp, array, sizeof(int) * n);
    #pragma omp parallel for
    for (int i = 0; i < n; i++){
        int count = 0;
        for (int j = 0; j < n; j++){
            if (temp[i] > temp[j])
                count++;
        }
        array[count] = temp[i];
    }
}

void test(int*A, int n){
    int* temp = (int*)malloc(sizeof(int) * n);
    int* array = (int*)malloc(sizeof(int) * n);
    printf("The size of vector is %d\n", n);
    struct timeval start, end;

    memcpy(array ,A, sizeof(int) * n);
    gettimeofday(&start, 0);
    quicksort(array, 0, n-1);
    gettimeofday(&end, 0);
    double time1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time1 /= 1000;
    printf("The quicksort time is %f\n", time1);

    memcpy(array ,A, sizeof(int) * n);
    gettimeofday(&start, 0);
    quicksort_omp(array, 0, n-1);
    gettimeofday(&end, 0);
    double time2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time2 /= 1000;
    printf("The quicksort_omp time is %f\n", time2);

    memcpy(array ,A, sizeof(int) * n);
    gettimeofday(&start, 0);
    mergesort(array, temp, 0, n-1);
    gettimeofday(&end, 0);
    double time3 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time3 /= 1000;
    printf("The mergesort time is %f\n", time3);

    memcpy(array ,A, sizeof(int) * n);
    gettimeofday(&start, 0);
    mergesort_omp(array, temp, 0, n-1);
    gettimeofday(&end, 0);
    double time4 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time4 /= 1000;
    printf("The mergesort_omp time is %f\n", time4);

    memcpy(array ,A, sizeof(int) * n);
    gettimeofday(&start, 0);
    my_sort(array, n);
    gettimeofday(&end, 0);
    double time5 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time4 /= 1000;
    printf("The my_sort time is %f\n", time5);

    memcpy(array ,A, sizeof(int) * n);
    gettimeofday(&start, 0);
    my_sort_omp(array, n);
    gettimeofday(&end, 0);
    double time6 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    time4 /= 1000;
    printf("The my_sort_omp time is %f\n", time6);

    free(temp);

    FILE *file = fopen("./time.txt", "a");
    fprintf(file, "%f,%f,%f,%f,%f\n", time1, time2, time3, time4,time5);
    fclose(file);
}

int main(){
    for(int n = 10; n <= 100000000; n *= 10){
        int* A = (int*)malloc(sizeof(int) * n);
        get_vector(A, n, 0);
        test(A, n);
        get_vector(A, n, 1);
        test(A, n);
        get_vector(A, n, 2);
        test(A, n);
    }
    return 0;
}
