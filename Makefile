CXX = clang++-14
CC = clang-14

# Common flags
OPTIMIZE_FLAGS = -O3 -funroll-loops -Wall -mavx2 -mfma

# C++ specific flags
CXXFLAGS = -std=c++20 $(OPTIMIZE_FLAGS)

# C specific flags (for OpenMP)
CFLAGS = $(OPTIMIZE_FLAGS) -fopenmp

# Linker flags (for OpenMP runtime)
LDFLAGS = -fopenmp

# List of C source files
C_SOURCES = naive.c naive_mp.c simd.c simd_mp.c

# Corresponding object files
C_OBJECTS = $(C_SOURCES:.c=.o)

# Target executable name
TARGET = euclidean_benchmark

# Default target
all: $(TARGET)

# Rule to link the C++ main file with C object files
$(TARGET): main.cc $(C_OBJECTS)
	$(CXX) $(CXXFLAGS) main.cc $(C_OBJECTS) -o $(TARGET) $(LDFLAGS)

# Rule to compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(C_OBJECTS)

.PHONY: all clean
