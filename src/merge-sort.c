#include <stdlib.h>

void merge(int *arr, int *temp, int leftPos, int rightPos, int rightFim) {
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

void sort(int *arr, int *temp, int left, int right) {
    if (left < right) {
        int edge = (left + right) / 2;
        sort(arr, temp, left, edge);
        sort(arr, temp, edge+1, right);
        merge(arr, temp, left, edge+1, right);
    }
}

int MergeSort(int *arr, int n) {
    int *temp = (int *)malloc(sizeof(int)*n);
    if (temp != NULL) {
        sort(arr, temp, 0, n-1);
        free(temp);
        return 1;
    }
    return 0;
}
