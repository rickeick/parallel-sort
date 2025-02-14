#include <omp.h>
#include <stdlib.h>

void mergeOMP(int *arr, int *temp, int leftPos, int rightPos, int rightFim) {
    int tempPos = leftPos;
    int leftEnd = rightPos - 1;
    int size = rightFim - leftPos + 1;
    while (leftPos <= leftEnd && rightPos <= rightFim) {
        if (arr[leftPos] <= arr[rightPos]) {
            temp[tempPos++] = arr[leftPos++];
        } else {
            temp[tempPos++] = arr[rightPos++];
        }
    }
    while (leftPos <= leftEnd) {
        temp[tempPos++] = arr[leftPos++];
    }
    while (rightPos <= rightFim) {
        temp[tempPos++] = arr[rightPos++];
    }
    for (int i=0; i<size; i++, rightFim--) {
        arr[rightFim] = temp[rightFim];
    }
}

void sortOMP(int *arr, int *temp, int left, int right, int depth) {
    if (left < right) {
        int edge = (left+right) / 2;
        if (depth > 0) {
            #pragma omp parallel sections
            {
                #pragma omp section
                sortOMP(arr, temp, left, edge, depth-1);
                #pragma omp section
                sortOMP(arr, temp, edge+1, right, depth-1);
            }
        } else {
            sortOMP(arr, temp, left, edge, depth-1);
            sortOMP(arr, temp, edge+1, right, depth-1);
        }
        mergeOMP(arr, temp, left, edge+1, right);
    }
}

int MergeSortOMP(int *arr, int n, int t) {
    int *temp = (int *)malloc(sizeof(int) * n);
    if (temp != NULL) {
        omp_set_num_threads(t);
        #pragma omp parallel
        {
            #pragma omp single
            sortOMP(arr, temp, 0, n - 1, omp_get_max_threads());
        }
        free(temp);
        return 1;
    }
    return 0;
}
