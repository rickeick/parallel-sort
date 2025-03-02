#include <omp.h>
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

void sort(int *arr, int *temp, int left, int right, int depth) {
    if (left < right) {
        int mid = (left+right) / 2;
        if (depth > 0) {
            #pragma omp task shared(arr, temp)
            sort(arr, temp, left, mid, depth-1);
            #pragma omp task shared(arr, temp)
            sort(arr, temp, mid+1, right, depth-1);
            #pragma omp taskwait
        } else {
            sort(arr, temp, left, mid, depth-1);
            sort(arr, temp, mid+1, right, depth-1);
        }
        merge(arr, temp, left, mid+1, right);
    }
}

int MergeSortOMP(int *arr, int size, int t) {
    int *temp = (int *)malloc(sizeof(int)*size);
    if (temp != NULL) {
        omp_set_num_threads(t);
        #pragma omp parallel
        {
            #pragma omp single
            sort(arr, temp, 0, size-1, t);
        }
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
    if (argc != 4) {
        printf("Parâmetros:\n");
        printf("\t<execuções: int>\n");
        printf("\t<tamanho: int>\n");
        printf("\t<threads: int>\n");
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
    int p = atoi(argv[3]);
    if (p < 1) {
        printf("Parâmetro <threads> deve ser maior que 1!\n");
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
    printf("Executando %d vezes o MergeSort OpenMP num vetor[%d] e %d threads:\n",
        execs, size, p
    );
    for (size_t i=0; i<execs; i++) {
        for (size_t j=0; j<size; j++) {
            arr[j] = rand();
        }
        init = clock();
        ret = MergeSortOMP(arr, size, p);
        end = clock();
        if (ret == 0) {
            printf("Falha ao executar algoritmo de ordenação!\n");
            free(arr);
            return 3;
        }
        for (size_t j=0; j<size-1; j++) {
            if (arr[j] > arr[j+1]) {
                printf("Falha na ordenação da execução %d!\n", i);
                free(arr);
                return 3;
            }
        }
        times[i] = ((double)(end-init)) / CLOCKS_PER_SEC;
        printf("\tExecução %d/%d concluída em %lf s\n", i+1, execs, times[i]);
    }
    
    FILE *file;
    char filepath[64];
    snprintf(filepath, 64, "logs/test-%dE-A2-%d-%dP.log", execs, size, p);
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
