#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "lru_cache.hpp"
#include "perfect_cache.hpp"

int SlowGetPage(int key) {
    
    return key;
}

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        std::cerr << "FAIL! Usage: ./exec filename" << std::endl;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Error: can't open file" << std::endl;
        return 1;
    }

    size_t cache_size   = 0; 
    size_t num_elements = 0;

    infile >> cache_size >> num_elements;

    std::vector<int> numbers;
    int num = 0;
    while (infile >> num) {
        numbers.push_back(num);
    }

    infile.close();

    auto start_time_perfect = std::chrono::high_resolution_clock::now();

    Cache::PerfectCache<int> perfect_cache(cache_size, numbers);

    for (size_t i = 0; i < num_elements; i++) {
        perfect_cache.LookupUpdate(numbers[i], SlowGetPage);
    }

    auto end_time_perfect = std::chrono::high_resolution_clock::now();
    auto duration_perfect = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_perfect - start_time_perfect);

    auto start_time_lru = std::chrono::high_resolution_clock::now();

    Cache::LRUCache<int> lru_cache(cache_size);

    for (size_t i = 0; i < num_elements; i++) {
        lru_cache.LookupUpdate(numbers[i], SlowGetPage);
    }

    auto end_time_lru = std::chrono::high_resolution_clock::now();
    auto duration_lru = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_lru - start_time_lru);

    std::cout << "Perfect time: " << duration_perfect.count() << " ms" << std::endl;
    std::cout << "LRU time: " << duration_lru.count() << " ms" << std::endl;
    std::cout << "LRU gain in speed: " << double(duration_perfect.count()) / double(duration_lru.count()) << "\n\n";

    return 0;
}
