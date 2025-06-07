#include <stdlib.h>
#include <math.h>

double naive(double* a, double* b, size_t len) {
    double distance = 0.0;
    for (size_t i = 0; i < len; i++) {
        double diff = a[i] - b[i];
        distance += diff * diff;
    }
    return sqrt(distance);
}