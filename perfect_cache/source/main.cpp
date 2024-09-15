#include "perfect_cache.hpp"

int SlowGetPage(int key) {
    
    return key;
}

int main() {

    size_t capacity = 0;
    size_t num_elem = 0;

    std::vector<int> keys;

    std::cout << std::endl << "Enter cache size and number of elements" << std::endl;
    std::cin >> capacity >> num_elem;

    int elem = 0;

     for (size_t i = 0; i < num_elem; i++) {
        std::cin >> elem;

        keys.push_back(elem);
    }

    Cache::PerfectCache<int> cache(capacity, keys);

    size_t num_hits = 0;

    for (size_t i = 0; i < num_elem; i++) {
        if (cache.LookupUpdate(keys[i], SlowGetPage)) {
            num_hits++;
        }
    }

    std::cout << "--------------------------------" << "\n\n";
    std::cout << "num hits = " << num_hits << std::endl;

    return 0;
}