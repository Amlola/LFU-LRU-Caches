#include <string>
#include <cstring>
#include <string_view>
#include "lru_cache.hpp"

int SlowGetPage(const std::string& key) {
    return key.length();
}

int RecursiveFib(int n, Cache::LRUCache<int, int>& cache) {

    if (n <= 2) {
        return 1;
    }

    cache.LookupUpdate(n, [&](int key) {

        return RecursiveFib(key - 1, cache) + RecursiveFib(key - 2, cache);
    });

    return cache.GetCachedValue(n);
}

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        std::cerr << "FAIL! Usage: ./build/lru_cache fib or ./build/lru_cache str" << std::endl;
        return -1;
    }

    else {
        size_t capacity = 0;
    
        if (!strcmp(argv[1], "len")) {
            size_t num_elem = 0;

            std::cout << std::endl << "Enter cache size and number of elements" << std::endl;
            std::cin >> capacity >> num_elem;

            Cache::LRUCache<int, std::string> cache(capacity);

            std::string elem = "";

            size_t num_hits = 0;

            for (size_t i = 0; i < num_elem; i++) {
                std::cin >> elem;

                if (cache.LookupUpdate(elem, SlowGetPage)) {
                    num_hits++;
                }
            }

            std::cout << "--------------------------------" << "\n\n";
            std::cout << "num hits = " << num_hits << std::endl;
        }

        else if (!strcmp(argv[1], "fib")) {
            int fib_num = 0;

            std::cout << std::endl << "Enter cache size and Fibonacci number" << std::endl;
            std::cin >> capacity >> fib_num;

            Cache::LRUCache<int> fib_cache(capacity);

            int result = RecursiveFib(fib_num, fib_cache);

            std::cout << "--------------------------------" << "\n\n";
            std::cout << "result = " << result << std::endl;
        }

        else {
            std::cerr << "Error: unknown argument" << std::endl;
            return -1;
        }
    }

    return 0;
}