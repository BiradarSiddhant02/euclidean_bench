#include <immintrin.h>
#include <math.h>

double simd(double *a, double *b, size_t len) {
    __m256d distance_vec = _mm256_setzero_pd();

    size_t i;
    for (i = 0; i + 3 < len; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d diff = _mm256_sub_pd(va, vb);
        __m256d sq = _mm256_mul_pd(diff, diff);
        distance_vec = _mm256_add_pd(distance_vec, sq);
    }

    // Horizontal add of 4 doubles
    double tmp[4];
    _mm256_storeu_pd(tmp, distance_vec);
    double distance = tmp[0] + tmp[1] + tmp[2] + tmp[3];

    // Handle remainder
    for (; i < len; ++i) {
        double d = a[i] - b[i];
        distance += d * d;
    }

    return sqrt(distance);
}