#include <any>
#include <cstddef>
#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <fstream>
#include "lfu_cache.hpp"
#include "perfect_cache.hpp"
#include "lru_cache.hpp"

using TestType = std::pair<std::vector<size_t>, std::vector<std::vector<int>>>;
using KeysType = std::vector<std::vector<int>>;

int SlowGetPage(int key) {
    
    return key;
}

TestType GetAllKeys() {

    const std::string file_name = "../data/test.txt";
    std::ifstream file(file_name);

    KeysType all_keys;
    size_t cache_size = 0; 
    size_t num_elems  = 0;

    int key = 0;

    std::vector<size_t> all_keys_size;

    if (file.is_open()) {
        while (file >> cache_size >> num_elems) {
            std::vector<int> keys;

            all_keys_size.push_back(cache_size);

            for (size_t i = 0; i < num_elems; i++) {
                if (file >> key) {
                    keys.push_back(key);
                } else {
                    throw std::runtime_error("Uknown Error");
                }
            }

            all_keys.push_back(keys);
        }

        file.close();

    } else {
        throw std::runtime_error("Can't open file");
    }
    
    return std::make_pair(all_keys_size, all_keys);
}

std::vector<size_t> GetExpectedHits(const std::string& filename) {

    std::ifstream file(filename);
    std::vector<size_t> expected_hits;
    int hits = 0;

    if (file.is_open()) {
        while (file >> hits) {
            expected_hits.push_back(hits);
        }
        file.close();
    } else {
        throw std::runtime_error("Can't open file");
    }

    return expected_hits;
}

template <typename CacheT>
size_t CountHits(CacheT& cache, const std::vector<int>& cur_keys, const size_t& cur_keys_size) {

    size_t num_hits = 0;

    for (size_t key = 0; key < cur_keys_size; key++) {
        if (cache.LookupUpdate(cur_keys[key], SlowGetPage)) {
            num_hits++;
        }
    }

    return num_hits;
}

template <typename CacheT> 
std::vector<size_t> GetCacheHits() {

    const TestType& all_keys_and_size = GetAllKeys();

    const std::vector<size_t>& all_keys_size = all_keys_and_size.first;
    const KeysType& all_keys                 = all_keys_and_size.second;

    const size_t& keys_size = all_keys.size();

    std::vector<size_t> all_hits;

    for (size_t i = 0; i < keys_size; i++) {
        const std::vector<int>& cur_keys = all_keys[i];
        const size_t& cur_keys_size = cur_keys.size();

        size_t num_hits = 0;

        if constexpr (!std::is_same<CacheT, Cache::PerfectCache<int>>::value) {
            CacheT cache(all_keys_size[i]);

            num_hits = CountHits(cache, cur_keys, cur_keys_size);
        }

        else {
            CacheT cache(all_keys_size[i], cur_keys);

            num_hits = CountHits(cache, cur_keys, cur_keys_size);
        }

        all_hits.push_back(num_hits);
    }

    return all_hits;
}

TEST(LRU_Test, CompareLruHits) {

    const std::vector<size_t>& lru_hits_received = GetCacheHits<Cache::LRUCache<int>>();
    const std::vector<size_t>& lru_hits_expected = GetExpectedHits("../expected_data/lru_check_hits.txt");

    const size_t& num_lru_hits_received = lru_hits_received.size();
    const size_t& num_lru_hits_expected = lru_hits_expected.size();

    ASSERT_EQ(num_lru_hits_received, num_lru_hits_expected) << "The number of expected results does not match";

    for (size_t i = 0; i < num_lru_hits_expected; i++) {
        EXPECT_EQ(lru_hits_expected[i], lru_hits_received[i]) << "Test error: " << i + 1;
    }
}

TEST(Pefect_Test, ComparePerfectHits) {

    const std::vector<size_t>& perfect_hits_received = GetCacheHits<Cache::PerfectCache<int>>();
    const std::vector<size_t>& perfect_hits_expected = GetExpectedHits("../expected_data/perfect_check_hits.txt");

    const size_t& num_perfect_hits_received = perfect_hits_received.size();
    const size_t& num_perfect_hits_expected = perfect_hits_expected.size();

    ASSERT_EQ(num_perfect_hits_received, num_perfect_hits_expected) << "The number of expected results does not match";

    for (size_t i = 0; i < num_perfect_hits_expected; i++) {
        EXPECT_EQ(perfect_hits_expected[i], perfect_hits_received[i]) << "Test error: " << i + 1;
    }
}

TEST(LFU_Test, CompareLFUHits) {

    const std::vector<size_t>& lfu_hits_received = GetCacheHits<Cache::LFUCache<int>>();
    const std::vector<size_t>& lfu_hits_expected = GetExpectedHits("../expected_data/lfu_check_hits.txt");

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