#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "lfu_cache.hpp"
#include "perfect_cache.hpp"

int SlowGetPage(int key) {
    
    return key;
}

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        std::cerr << "FAIL! Usage: sh bench.sh filename" << std::endl;
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

    size_t num_hits_perfect = 0;
    size_t num_hits_lfu = 0;

    auto start_time_perfect = std::chrono::high_resolution_clock::now();

    Cache::PerfectCache<int> perfect_cache(cache_size, numbers);

    for (const auto& cur_key : numbers) {
        if (perfect_cache.LookupUpdate(cur_key, SlowGetPage)) {
            num_hits_perfect++;
        }
    }

    auto end_time_perfect = std::chrono::high_resolution_clock::now();
    auto duration_perfect = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_perfect - start_time_perfect);

    auto start_time_lfu = std::chrono::high_resolution_clock::now();

    Cache::LFUCache<int> lfu_cache(cache_size);

    for (const auto& cur_key : numbers) {
        if (lfu_cache.LookupUpdate(cur_key, SlowGetPage)) {
            num_hits_lfu++;
        }
    }

    auto end_time_lfu = std::chrono::high_resolution_clock::now();
    auto duration_lfu = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_lfu - start_time_lfu);

    std::cout << "Perfect time: " << duration_perfect.count() << " ms\n";
    std::cout << "LFU time: " << duration_lfu.count() << " ms\n";
    std::cout << "LFU gain in speed: " << double(duration_perfect.count()) / double(duration_lfu.count()) << "\n";
    std::cout << "Perfect hits = " << num_hits_perfect << "\n";
    std::cout << "LFU hits = " << num_hits_lfu << "\n";

    if (num_hits_lfu != 0)
        std::cout << "Hit ratio (perfect / LFU) = " << double(num_hits_perfect) / double(num_hits_lfu) << "\n\n";

    return 0;
}
