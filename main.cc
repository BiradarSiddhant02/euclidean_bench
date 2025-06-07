#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <string>
#include <functional>
#include <iomanip>    // For std::fixed and std::setprecision
#include <cstdlib>    // For atoi
// #include <omp.h>   // For OpenMP functions if needed by C files

extern "C" {
    double naive(double* p1, double* p2, size_t n);
    double naive_mp(double* p1, double* p2, size_t n);
    double simd(double* p1, double* p2, size_t n);
    double simd_mp(double* p1, double* p2, size_t n);
}

// Helper function to generate random double vectors
std::vector<double> generate_random_vector(size_t size) {
    std::vector<double> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    }
    return vec;
}

// Timer function
long long time_function(std::function<double()> func, int trials) {
    std::vector<long long> durations;
    for (int i = 0; i < trials; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func(); // Call the function
        auto end = std::chrono::high_resolution_clock::now();
        durations.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }
    return std::accumulate(durations.begin(), durations.end(), 0LL) / durations.size();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <num_threads> <num_trials>" << std::endl;
        return 1;
    }

    int num_threads = std::atoi(argv[1]);
    int num_trials = std::atoi(argv[2]);

    if (num_threads <= 0 || num_trials <= 0) {
        std::cerr << "Number of threads and trials must be positive integers." << std::endl;
        return 1;
    }

    // omp_set_num_threads(num_threads); // Set number of threads for OpenMP functions

    // Example data
    int vector_dim = 1024 * 1024; // Dimension of the vectors
    std::vector<double> vec1 = generate_random_vector(vector_dim);
    std::vector<double> vec2 = generate_random_vector(vector_dim);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Benchmarking with " << num_threads << " threads and " << num_trials << " trials per function." << std::endl;
    std::cout << "Vector dimension: " << vector_dim << std::endl << std::endl;

    // Header for the table
    std::cout << std::left << std::setw(30) << "Function" << std::right << std::setw(15) << "Avg Time (ns)" << std::setw(15) << "Distance" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // --- Naive Euclidean Distance ---
    double dist_naive = 0;
    auto naive_func = [&]() {
        dist_naive = naive(vec1.data(), vec2.data(), vector_dim);
        return dist_naive;
    };
    long long avg_time_naive = time_function(naive_func, num_trials);
    std::cout << std::left << std::setw(30) << "Naive Euclidean Distance" << std::right << std::setw(15) << avg_time_naive << std::setw(15) << dist_naive << std::endl;

    // --- Naive MP Euclidean Distance ---
    double dist_naive_mp = 0;
    auto naive_mp_func = [&]() {
        dist_naive_mp = naive_mp(vec1.data(), vec2.data(), vector_dim);
        return dist_naive_mp;
    };
    long long avg_time_naive_mp = time_function(naive_mp_func, num_trials);
    std::cout << std::left << std::setw(30) << "Naive MP Euclidean Distance" << std::right << std::setw(15) << avg_time_naive_mp << std::setw(15) << dist_naive_mp << std::endl;

    // --- SIMD Euclidean Distance ---
    double dist_simd = 0;
    auto simd_func = [&]() {
        dist_simd = simd(vec1.data(), vec2.data(), vector_dim);
        return dist_simd;
    };
    long long avg_time_simd = time_function(simd_func, num_trials);
    std::cout << std::left << std::setw(30) << "SIMD Euclidean Distance" << std::right << std::setw(15) << avg_time_simd << std::setw(15) << dist_simd << std::endl;

    // --- SIMD MP Euclidean Distance ---
    double dist_simd_mp = 0;
    auto simd_mp_func = [&]() {
        dist_simd_mp = simd_mp(vec1.data(), vec2.data(), vector_dim);
        return dist_simd_mp;
    };
    long long avg_time_simd_mp = time_function(simd_mp_func, num_trials);
    std::cout << std::left << std::setw(30) << "SIMD MP Euclidean Distance" << std::right << std::setw(15) << avg_time_simd_mp << std::setw(15) << dist_simd_mp << std::endl;

    return 0;
}
