#include <immintrin.h>
#include <omp.h>
#include <math.h>

double simd_mp(double *a, double *b, size_t len) {
    double distance = 0.0;

    #pragma omp parallel
    {
        __m256d distance_vec = _mm256_setzero_pd();
        double local_sum = 0.0;
        size_t i;
        const size_t simd_limit = len - (len % 4); // Calculate limit for SIMD operations

        #pragma omp for nowait
        for (i = 0; i < simd_limit; i += 4) { // Modified loop condition
            __m256d va = _mm256_loadu_pd(&a[i]);
            __m256d vb = _mm256_loadu_pd(&b[i]);
            __m256d diff = _mm256_sub_pd(va, vb);
            __m256d sq = _mm256_mul_pd(diff, diff);
            distance_vec = _mm256_add_pd(distance_vec, sq);
        }

        // Horizontal add of the SIMD vector
        double tmp[4];
        _mm256_storeu_pd(tmp, distance_vec);
        local_sum += tmp[0] + tmp[1] + tmp[2] + tmp[3];

        // Handle remaining elements
        for (; i < len; ++i) { // This loop now correctly starts from simd_limit
            double d = a[i] - b[i];
            local_sum += d * d;
        }

        // Atomically accumulate to global sum
        #pragma omp atomic
        distance += local_sum;
    }

    return sqrt(distance);
}
