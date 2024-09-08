#include "lru_cache.hpp"

int SlowGetPage(int key) {
    return key;
}

int main() {

    size_t capacity = 0;
    size_t num_elem = 0;

    std::cout << "Enter cache size and number of elements" << std::endl;
    std::cin >> capacity >> num_elem;

    Cache::LRUCache<int> cache(capacity);

    size_t num_hits = 0;

    for (size_t i = 0; i < num_elem; i++) {
        int elem = 0;
        std::cin >> elem;

        if (cache.LookupUpdate(elem, SlowGetPage))
            num_hits++;
    }

    std::cout << std::endl << "Number hits = " << num_hits << std::endl;

    return 0;
}