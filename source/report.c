#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "sllist.h"
#include "utils.c"

typedef struct _logfile_ {
    int alg;
    int exes;
    int size;
    int p;
    double *times;
} LogFile;

LogFile *readLog(char *filepath) {
    int exes, alg, size, p;
    int n = sscanf(filepath, "./logs/test-%dE-A%d-%d-%dP.log",
        &exes, &alg, &size, &p
    );
    if (n == 4) {
        LogFile *logfile = (LogFile *)malloc(sizeof(LogFile));
        if (logfile != NULL) {
            FILE *file = fopen(filepath, "r");
            double *times = (double *)malloc(sizeof(double)*exes);
            if (file != NULL && times != NULL) {
                for (int i=0; i<exes; i++) {
                    fscanf(file, "%lf", &times[i]);
                }
                logfile->alg = alg;
                logfile->exes = exes;
                logfile->size = size;
                logfile->p = p;
                logfile->times = times;
                fclose(file);
                return logfile;
            }
            free(logfile);
        }
    }
    return NULL;
}

void freeLog(void *data) {
    if (data != NULL) {
        LogFile *logfile = (LogFile *)data;
        free(logfile->times);
        free(logfile);
    }
}

int main(int argc, const char *argv[]) {
    #ifdef _WIN32
        system("chcp 65001 > NUL");
    #endif
    if (argc != 2) {
        printf("Parâmetros:\n");
        printf("\t<algoritmo_serial>\n");
        printf("\t<algoritmo_paralelo>\n");
        printf("Algoritmos Seriais:\n");
        printf("\t[A0] MergeSort\n");
        printf("\t[A5] BubbleSort\n");
        printf("Algoritmos Paralelos:\n");
        printf("\t[A1] MergeSort MPI\n");
        printf("\t[A2] MergeSort OMP\n");
        printf("\t[A3] MergeSort Odd-Even\n");
        printf("\t[A4] BubbleSort Odd-Even\n");
        return 1;
    }

    int s=0, p=0;
    char filepath[64];
    struct dirent *entry;
    const char *alg_s = argv[1];
    const char *alg_p = argv[2];
    DIR *dir = opendir("./logs");
    if (dir == NULL) {
        printf("Falha ao abrir diretório!\n");
        return 1;
    }
    SLList *logs_serial = sllCreate();
    SLList *logs_parallel = sllCreate();
    if (logs_serial == NULL || logs_parallel == NULL) {
        printf("Falha ao alocar memória!\n");
        closedir(dir);
        return 2;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".log") != NULL) {
            snprintf(filepath, 64, "./logs/%s", entry->d_name);
            if (strstr(entry->d_name, alg_s) != NULL) {
                sllAppend(logs_serial, (void *)readLog(filepath)); s++;
            } else if (strstr(entry->d_name, alg_p) != NULL) {
                sllAppend(logs_parallel, (void *)readLog(filepath)); p++;
            }
        }
    }
    closedir(dir);
    free(entry);
    
    LogFile *serial;
    LogFile *parallel;
    double speedup, efficiency;
    double avg_serial, sd_serial;
    double avg_parallel, sd_parallel;
    snprintf(filepath, 64, "./reports/report-%s-%s.txt", alg_s, alg_p);
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("\nFalha ao abrir arquivo!\n");
        sllDestroy(logs_parallel, freeLog);
        sllDestroy(logs_serial, freeLog);
        return 2;
    }
    fprintf(file, "Relatório para %s/%s\n", alg_s, alg_p);
    for (size_t i=0; i<s; i++) {
        serial = (LogFile *)sllGetNext(logs_serial);
        avg_serial = avg(serial->times, serial->exes);
        sd_serial = sd(serial->times, serial->exes, avg_serial);
        fprintf(file, "\nPara um Vetor com %d de tamanho:\n", serial->size);
        fprintf(file, "\tMédia Serial: %lf\n", avg_serial);
        fprintf(file, "\tDesvio Serial: %lf\n", sd_serial);
        for (size_t j=0; j<p/s; j++) {
            parallel = (LogFile *)sllGetNext(logs_parallel);
            avg_parallel = avg(parallel->times, parallel->exes);
            sd_parallel = sd(parallel->times, parallel->exes, avg_parallel);
            speedup = avg_serial / avg_parallel;
            efficiency = speedup / parallel->p;
            fprintf(file, "\tCom %d Processos/Threads:\n", parallel->p);
            fprintf(file, "\t\tMédia: %lf\n", avg_parallel);
            fprintf(file, "\t\tDesvio: %lf\n", sd_parallel);
            fprintf(file, "\t\tSpeedUp: %lf\n", speedup);
            fprintf(file, "\t\tEficiência: %lf\n", efficiency);
        }
    }
    printf("Relatório para  %s/%s salvo!", alg_s, alg_p);
    sllDestroy(logs_parallel, freeLog);
    sllDestroy(logs_serial, freeLog);
    freeLog(parallel);
    freeLog(serial);
    fclose(file);
    return 0;
}
