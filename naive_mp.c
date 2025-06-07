#include <math.h>
#include <omp.h>

double naive_mp(double* a, double* b, size_t len) {
    double distance = 0.0;

    #pragma omp parallel
    {
        double partial_distance = 0.0;

        #pragma omp for nowait 
        for (size_t i = 0; i < len; i++) {
            double diff = a[i] - b[i];
            partial_distance += diff * diff;
        }

        #pragma omp atomic
        distance += partial_distance;
    }

    return sqrt(distance);
}