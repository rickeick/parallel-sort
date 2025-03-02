#include <mpi.h>
#include <time.h>
#include <stdio.h>
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

void mergeSort(int *arr, int *temp, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, temp, left, mid);
        mergeSort(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid + 1, right);
    }
}

int main(int argc, char *argv[]) {
    int p, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    srand(time(NULL));
    #ifdef _WIN32
        system("chcp 65001 > NUL");
    #endif

    if (rank == 0) {
        if (argc != 4) {
            printf("Parâmetros:\n");
            printf("\t<execuções: int>\n");
            printf("\t<atual: int>\n");
            printf("\t<tamanho: int>\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
    }

    int execs = atoi(argv[1]);
    int cur = atoi(argv[2]);
    int size = atoi(argv[3]);
    if (rank == 0) {
        if (execs < 1) {
            printf("Parâmetro <execuções> deve ser maior que 1!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        if (size <= 0) {
            printf("Parâmetro <tamanho> deve ser maior que 0!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        if (cur < 1 && cur > execs) {
            printf("Parâmetro <atual> deve está entre 1 e <execuções>!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
    }

    double time;
    clock_t init, end;
    int *arr = (int*)malloc(sizeof(int)*size);
    if (rank == 0) {
        if (arr == NULL) {
            printf("Falha ao alocar memória!\n");
            MPI_Abort(MPI_COMM_WORLD, 2);
            return 2;
        }
        for (size_t j = 0; j < size; j++) {
            arr[j] = rand();
        }
        if (cur == 1) {
            printf("Executando %d vezes o MergeSort MPÌ num vetor[%d] e %d threads:\n",
                execs, size, p
            );
        }
        init = clock();
    }

    int n = size / p;
    int *sub_arr = malloc(n * sizeof(int));
    MPI_Scatter(arr, n, MPI_INT, sub_arr, n, MPI_INT, 0, MPI_COMM_WORLD);
    
    int *tmp_arr = malloc(n * sizeof(int));
    mergeSort(sub_arr, tmp_arr, 0, n - 1);
    
    int *sorted = NULL;
    if (rank == 0) {
        sorted = malloc(size * sizeof(int));
    }
    MPI_Gather(sub_arr, n, MPI_INT, sorted, n, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        int *other_arr = malloc(size * sizeof(int));
        mergeSort(sorted, other_arr, 0, size - 1);
        end = clock();
        for (size_t i=0; i<size-1; i++) {
            if (sorted[i] > sorted[i+1]) {
                printf("Falha na ordenação!\n");
                MPI_Abort(MPI_COMM_WORLD, 3);
                free(arr);
                return 3;
            }
        }
        time = ((double)(end-init)) / CLOCKS_PER_SEC;
        printf("\tExecução %d/%d concluída em %lf s\n", cur, execs, time);
        free(other_arr);
        free(sorted);
    }

    if (rank == 0) {
        FILE *file;
        char filepath[64];
        snprintf(filepath, 64, "logs/test-%dE-A1-%d-%dP.log", execs, size, p);
        file = fopen(filepath, "a");
        if (file == NULL) {
            printf("Falha ao abrir arquivo de log!\n");
            MPI_Abort(MPI_COMM_WORLD, 2);
            free(arr);
            return 2;
        }
        fprintf(file, "%lf\n", time);
        if (cur == execs) {
            printf("Arquivo de LOG salvo!\n");
        }
        fclose(file);
    }

    free(sub_arr);
    free(tmp_arr);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    free(arr);
    return 0;
}
