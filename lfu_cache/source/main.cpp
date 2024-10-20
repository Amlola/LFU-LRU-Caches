#include "lfu_cache.hpp"

int SlowGetPage(int key) {

    return key;
}

int main() {

    try {
        size_t capacity = 0;
        size_t num_elem = 0;

        std::cin >> capacity >> num_elem;


            Cache::LFUCache<int> cache(capacity);

            int elem = 0;

            size_t num_hits = 0;

            for (size_t i = 0; i < num_elem; i++) {
                std::cin >> elem;

                if (cache.LookupUpdate(elem, SlowGetPage)) {
                    num_hits++;
                }
            }

            std::cout << num_hits << "\n";

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    return 0;
}