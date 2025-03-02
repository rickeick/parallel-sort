#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void merge(int *arr, int low, int size, int gap) {
    if (gap <= 0) return;
    int step = gap * 2;
    if (step < size) {
        merge(arr, low, size, step);
        merge(arr, low + gap, size, step);
        for (int i = low + gap; i + gap < low + size; i += step) {
            if (arr[i] > arr[i + gap]) {
                swap(&arr[i], &arr[i + gap]);
            }
        }
    } else {
        if (arr[low] > arr[low + gap]) {
            swap(&arr[low], &arr[low + gap]);
        }
    }
}

void sort(int *arr, int low, int size) {
    if (size <= 1) return;
    int mid = size / 2;
    #pragma omp task shared(arr) firstprivate(low, mid)
    sort(arr, low, mid);
    #pragma omp task shared(arr) firstprivate(low, mid)
    sort(arr, low + mid, mid);
    #pragma omp taskwait
    merge(arr, low, size, 1);
}

int MergeSortOddEven(int *arr, int size, int t) {
    #pragma omp parallel num_threads(t)
    {
        #pragma omp single nowait
        {
            sort(arr, 0, size);
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    #ifdef _WIN32
        system("chcp 65001 > NUL");
    #endif
    if (argc < 4) {
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
    printf("Executando %d vezes o MergeSort Odd-Even num vetor[%d] e %d threads:\n",
        execs, size, p
    );
    for (size_t i=0; i<execs; i++) {
        for (size_t j=0; j<size; j++) {
            arr[j] = rand();
        }
        init = clock();
        ret = MergeSortOddEven(arr, size, p);
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
    snprintf(filepath, 64, "logs/test-%dE-A3-%d-%dP.log",
        execs, size, p
    );
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
