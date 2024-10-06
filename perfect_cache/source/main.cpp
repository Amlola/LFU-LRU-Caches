#include "perfect_cache.hpp"

int SlowGetPage(int key) {
    
    return key;
}

int main() {

    size_t capacity = 0;
    size_t num_elem = 0;

    std::vector<int> keys;

    std::cin >> capacity >> num_elem;

    int elem = 0;

    for (size_t i = 0; i < num_elem; i++) {
        std::cin >> elem;

        keys.push_back(elem);
    }

    try {
        Cache::PerfectCache<int> cache(capacity, keys);

        size_t num_hits = 0;

        for (const auto& cur_key : keys) {
            if (cache.LookupUpdate(cur_key, SlowGetPage)) {
                num_hits++;
            }
        }

        std::cout << num_hits << "\n";

    } catch (const std::invalid_argument& err) {
        std::cerr << "Error: " << err.what() << "\n";
    }

    return 0;
}