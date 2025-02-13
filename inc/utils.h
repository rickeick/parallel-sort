#include <math.h>

double avg(double *times, int n) {
    double sum = 0.0;
    for (int i=0; i<n; i++) {
        sum += times[i];
    }
    return sum / n;
}

double sd(double *times, int n, double avg) {
    double sum = 0.0;
    for (int i=0; i<n; i++) {
        sum += pow(times[i]-avg, 2);
    }
    return sqrt(sum / n);
}
