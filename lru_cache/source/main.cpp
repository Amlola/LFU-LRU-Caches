#include <cstdlib>
#include <string>
#include <cstring>
#include <string_view>
#include "lru_cache.hpp"

int SlowGetPage(const std::string& key) {
    
    return key.length();
}

int RecursiveFib(int n, Cache::LRUCache<int>& cache) {

    if (n <= 2) {
        return 1;
    }

    cache.LookupUpdate(n, [&](int key) {

        return RecursiveFib(key - 1, cache) + RecursiveFib(key - 2, cache);
    });

    return cache.GetCachedValue(n);
}

int SlowGetPageNum(int key) {

    return key;
}

int main(int argc, const char* argv[]) {

    try {
        if (argc < 2) {
            size_t capacity = 0;
            size_t num_elem = 0;

            std::cin >> capacity >> num_elem;

            Cache::LRUCache<int> cache(capacity);

            size_t num_hits = 0;

            int elem = 0;

            for (size_t i = 0; i < num_elem; i++) {
                std::cin >> elem;

                if (cache.LookupUpdate(elem, SlowGetPageNum)) {
                    num_hits++;
                }
            }

            std::cout << num_hits << "\n";

        } else {
            size_t capacity = 0;
        
            if (std::string_view(argv[1]) == "len") {
                size_t num_elem = 0;

                std::cin >> capacity >> num_elem;

                Cache::LRUCache<int, std::string> cache(capacity);

                std::string elem;

                size_t num_hits = 0;

                for (size_t i = 0; i < num_elem; i++) {
                    std::cin >> elem;

                    if (cache.LookupUpdate(elem, SlowGetPage)) {
                        num_hits++;
                    }
                }

                std::cout << num_hits << "\n";

            } else if (std::string_view(argv[1]) == "fib") {
                int fib_num = 0;

                std::cin >> capacity >> fib_num;

                Cache::LRUCache<int> fib_cache(capacity);

                int result = RecursiveFib(fib_num, fib_cache);

                std::cout << result << "\n";

            } else {
                std::cerr << "FAIL! Usage: ./build/lru_cache fib or ./build/lru_cache len\n";
                return EXIT_FAILURE;
            }
        }

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    return 0;
}