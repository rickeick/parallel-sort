#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        int mid = (left+right) / 2;
        sort(arr, temp, left, mid);
        sort(arr, temp, mid+1, right);
        merge(arr, temp, left, mid+1, right);
    }
}

int MergeSort(int *arr, int size) {
    int *temp = (int *)malloc(sizeof(int)*size);
    if (temp != NULL) {
        sort(arr, temp, 0, size-1);
        free(temp);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    #ifdef _WIN32
        system("chcp 65001 > NUL");
    #endif
    if (argc != 3) {
        printf("Parâmetros:\n");
        printf("\t<execuções: int>\n");
        printf("\t<tamanho: int>\n");
        return 1;
    }
    int execs = atoi(argv[1]);
    if (execs < 1) {
        printf("Parâmetro <execuções> deve ser maior que 1!\n");
        return 1;
    }
    int size = atoi(argv[2]);
    if (size <= 0) {
        printf("Parâmetro <tamanho> deve ser maior que 0!\n");
        return 1;
    }

    int ret;
    clock_t init, end;
    double times[execs];
    int *arr = (int*)malloc(sizeof(int)*size);
    if (arr == NULL) {
        printf("Falha ao alocar memória!\n");
        return 2;
    }
    printf("Executando %d vezes o MergeSort Serial num vetor[%d]:\n", execs, size);
    for (size_t i=0; i<execs; i++) {
        for (size_t j=0; j<size; j++) {
            arr[j] = rand();
        }
        init = clock();
        ret = MergeSort(arr, size);
        end = clock();
        if (ret == 0) {
            printf("\nFalha ao executar algoritmo de execução!\n");
            free(arr);
            return 3;
        }
        for (size_t j=0; j<size-1; j++) {
            if (arr[j] > arr[j+1]) {
                printf("\nFalha na ordenação da execução %d!\n", i);
                free(arr);
                return 3;
            }
        }
        times[i] = ((double)(end-init)) / CLOCKS_PER_SEC;
        printf("\tExecução %d/%d concluída em %lf s\n", i+1, execs, times[i]);
    }
    
    FILE *file;
    char filepath[64];
    snprintf(filepath, 64, "logs/test-%dE-A0-%d-1P.log", execs, size);
    file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Falha ao abrir arquivo de log!\n");
        free(arr);
        return 2;
    }
    for (size_t j=0; j<execs; j++) {
        fprintf(file, "%lf\n", times[j]);
    }
    printf("Arquivo de LOG salvo!\n");
    fclose(file);
    free(arr);
    return 0;
}
