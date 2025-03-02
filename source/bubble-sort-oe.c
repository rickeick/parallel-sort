#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int BubbleSortOddEven(int* arr, int size, int p) {
    int is_sorted = 0;
    omp_set_num_threads(p);
    while (!is_sorted) {
        is_sorted = 1;
        #pragma omp parallel for
        for (int i = 1; i < size - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < size - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
            }
        }
        #pragma omp parallel for reduction(&&:is_sorted)
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                is_sorted = 0;
            }
        }
    }
    return 1;
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
    printf("Executando %d vezes o BubbleSort Odd-Even num vetor[%d] e %d threads:\n",
        execs, size, p
    );
    for (size_t i=0; i<execs; i++) {
        for (size_t j=0; j<size; j++) {
            arr[j] = rand();
        }
        init = clock();
        ret = BubbleSortOddEven(arr, size, p);
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
    snprintf(filepath, 64, "logs/test-%dE-A4-%d-%dP.log", execs, size, p);
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
