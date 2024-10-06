#include <cstddef>
#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "lfu_cache.hpp"
#include "perfect_cache.hpp"
#include "lru_cache.hpp"

int SlowGetPage(int key) {
    
    return key;
}

std::vector<std::vector<int>> GetAllKeys() {

    const std::string file_name = "../data/test.txt";

    std::ifstream file(file_name);
    std::vector<std::vector<int>> all_keys;
    size_t cache_size = 0; 
    size_t num_elems  = 0;

    int key = 0;

    if (file.is_open()) {
        while (file >> cache_size >> num_elems) {
            std::vector<int> keys;

            keys.push_back(cache_size);

            for (size_t i = 0; i < num_elems; i++) {
                if (file >> key) {
                    keys.push_back(key);
                } else {
                    std::cerr << "Uknown error\n";
                    break;
                }
            }

            all_keys.push_back(keys);
        }

        file.close();

    } else {
        std::cerr << "Can't open file: " << file_name << "\n";
    }
    
    return all_keys;
}

std::vector<int> GetExpectedHits(const std::string& filename) {

    std::ifstream file(filename);
    std::vector<int> expected_hits;
    int hits = 0;

    if (file.is_open()) {
        while (file >> hits) {
            expected_hits.push_back(hits);
        }
        file.close();
    } else {
        std::cerr << "Can't open file: " << filename << "\n";
    }

    return expected_hits;
}

std::vector<int> LruCacheGetHits() {

    const std::vector<std::vector<int>>& all_keys = GetAllKeys();

    const size_t& all_keys_size = all_keys.size();

    std::vector<int> all_hits;

    for (size_t i = 0; i < all_keys_size; i++) {
        const std::vector<int>& cur_keys = all_keys[i];
        const size_t& cur_keys_size = cur_keys.size();

        Cache::LRUCache<int> cache(cur_keys[0]);

        int num_hits = 0;

        for (size_t key = 1; key < cur_keys_size; key++) {
            if (cache.LookupUpdate(cur_keys[key], SlowGetPage)) {
                num_hits++;
            }
        }

        all_hits.push_back(num_hits);
    }

    return all_hits;
}

std::vector<int> LfuCacheGetHits() {

    const std::vector<std::vector<int>>& all_keys = GetAllKeys();

    size_t all_keys_size = all_keys.size();

    std::vector<int> all_hits;

    for (size_t i = 0; i < all_keys_size; i++) {
        const std::vector<int>& cur_keys = all_keys[i];
        const size_t& cur_keys_size = cur_keys.size();

        Cache::LFUCache<int> cache(cur_keys[0]);

        int num_hits = 0;

        for (size_t key = 1; key < cur_keys_size; key++) {
            if (cache.LookupUpdate(cur_keys[key], SlowGetPage)) {
                num_hits++;
            }
        }

        all_hits.push_back(num_hits);
    }

    return all_hits;
}

std::vector<int> PerfectCacheGetHits() {

    const std::vector<std::vector<int>>& all_keys = GetAllKeys();

    const size_t& all_keys_size = all_keys.size();

    std::vector<int> all_hits;

    for (size_t i = 0; i < all_keys_size; i++) {
        std::vector<int> cur_keys = all_keys[i];

        int cache_size = cur_keys[0];

        cur_keys.erase(cur_keys.begin());

        const size_t& cur_keys_size = cur_keys.size();

        Cache::PerfectCache<int> cache(cache_size, cur_keys);

        int num_hits = 0;

        for (size_t key = 0; key < cur_keys_size; key++) {
            if (cache.LookupUpdate(cur_keys[key], SlowGetPage)) {
                num_hits++;
            }
        }

        all_hits.push_back(num_hits);
    }

    return all_hits;
}

TEST(LRU_Test, CompareLruHits) {

    const std::vector<int>& lru_hits_received = LruCacheGetHits();
    const std::vector<int>& lru_hits_expected = GetExpectedHits("../expected_data/lru_check_hits.txt");

    const size_t& num_lru_hits_received = lru_hits_received.size();
    const size_t& num_lru_hits_expected = lru_hits_expected.size();

    ASSERT_EQ(num_lru_hits_received, num_lru_hits_expected) << "The number of expected results does not match";

    for (size_t i = 0; i < num_lru_hits_expected; i++) {
        EXPECT_EQ(lru_hits_expected[i], lru_hits_received[i]) << "Test error: " << i + 1;
    }
}

TEST(Pefect_Test, ComparePerfectHits) {

    const std::vector<int>& perfect_hits_received = PerfectCacheGetHits();
    const std::vector<int>& perfect_hits_expected = GetExpectedHits("../expected_data/perfect_check_hits.txt");

    const size_t& num_perfect_hits_received = perfect_hits_received.size();
    const size_t& num_perfect_hits_expected = perfect_hits_expected.size();

    ASSERT_EQ(num_perfect_hits_received, num_perfect_hits_expected) << "The number of expected results does not match";

    for (size_t i = 0; i < num_perfect_hits_expected; i++) {
        EXPECT_EQ(perfect_hits_expected[i], perfect_hits_received[i]) << "Test error: " << i + 1;
    }
}

TEST(LFU_Test, CompareLFUHits) {

    const std::vector<int>& lfu_hits_received = LfuCacheGetHits();
    const std::vector<int>& lfu_hits_expected = GetExpectedHits("../expected_data/lfu_check_hits.txt");

    const size_t& num_lfu_hits_received = lfu_hits_received.size();
    const size_t& num_lfu_hits_expected = lfu_hits_expected.size();

    ASSERT_EQ(num_lfu_hits_received, num_lfu_hits_expected) << "The number of expected results does not match";

    for (size_t i = 0; i < num_lfu_hits_expected; i++) {
        EXPECT_EQ(lfu_hits_expected[i], lfu_hits_received[i]) << "Test error: " << i + 1;
    }
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}