#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    #ifdef _WIN32
        system("chcp 65001 > NUL");
    #endif
    if (argc < 4 || argc > 5) {
        printf("Parâmetros:\n");
        printf("\t<execuções: int>\n");
        printf("\t<algoritmo: int>\n");
        printf("\t<tamanho: int>\n");
        printf("\t[processos: int]\n");
        printf("Algoritmos:\n");
        for (size_t i=0; i<5; i++) {
            printf("\t[%d] %s\n", i, ALGS[i]);
        }
        return 1;
    }
    int execs = atoi(argv[1]);
    if (execs < 1) {
        printf("Parâmetro <execuções> deve ser maior que 1!\n");
        return 1;
    }
    int alg = atoi(argv[2]);
    if (alg < 0 || alg > 4) {
        printf("Parâmetro <algoritmo> deve está entre 0 e 4!\n");
        return 1;
    }
    int size = atoi(argv[3]);
    if (size < 0) {
        printf("Parâmetro <tamanho> deve ser maior que 0!\n");
        return 1;
    }
    int p = 1;
    if (alg != 0) {
        p = atoi(argv[4]);
        if (p < 0) {
            printf("Parâmetro <processos> deve ser maior que 0!\n");
            return 1;
        }
    }

    int ret;
    clock_t init, end;
    double times[execs];
    int *arr = (int*)malloc(sizeof(int)*size);
    if (arr == NULL) {
        printf("Falha ao alocar memória!\n");
        return 2;
    }
    printf("Executando %d vezes o %s num vetor[%d] e %d processo(s)...",
        execs, ALGS[alg], size, p
    );
    for (size_t i=0; i<execs; i++) {
        for (size_t j=0; j<size; j++) {
            arr[j] = rand();
        }
        init = clock();
        switch (alg) {
            case 0: ret = MergeSort(arr, size); break;
            //case 1: ret = MergeSortMPI(); break;
            case 2: ret = MergeSortOMP(arr, size, p); break;
            //case 3: ret = MergeSortOddEven(); break;
            //case 4: ret = BubbleSortOddEven(); break;
        }
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
    }
    
    FILE *file;
    char filepath[64];
    snprintf(filepath, 64, "logs/test-%dE-A%d-%d-%dP.log",
        execs, alg, size, p
    );
    file = fopen(filepath, "w");
    if (file == NULL) {
        printf("\nFalha ao abrir arquivo de log!\n");
        free(arr);
        return 2;
    }
    for (size_t j=0; j<execs; j++) {
        fprintf(file, "%lf\n", times[j]);
    }
    printf("\t[OK]\n");
    fclose(file);
    free(arr);
    return 0;
}
