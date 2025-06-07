## Build
```bash
make        # compile and link all
make clean  # delete all compiled binaries
```

## Run Benchmark
```bash
# num_threads: number of parallel tasks
# num_trials: number of trials to run
./euclidean_benchmark <num_threads> <num_trials>
```

## Results
```
Benchmarking with 8 threads and 100 trials per function.
Vector dimension: 1048576

Function                        Avg Time (ns)       Distance
------------------------------------------------------------
Naive Euclidean Distance              1381759         417.83
Naive MP Euclidean Distance            568977         417.83
SIMD Euclidean Distance                704167         417.83
SIMD MP Euclidean Distance             569810         417.83

```