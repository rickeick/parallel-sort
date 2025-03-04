#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void BubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        int swapped = 0;
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
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
    printf("Executando %d vezes o BubbleSort Serial num vetor[%d]:\n", execs, size);
    for (size_t i=0; i<execs; i++) {
        for (size_t j=0; j<size; j++) {
            arr[j] = rand();
        }
        init = clock();
        BubbleSort(arr, size);
        end = clock();
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
    snprintf(filepath, 64, "logs/test-%dE-A5-%d-1P.log", execs, size);
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