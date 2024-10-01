#ifndef PERFECT_CACHE_HPP
#define PERFECT_CACHE_HPP

#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <map>
#include <vector>

namespace Cache {

    template<typename T, typename Keyt = int> 
    class PerfectCache final {

        size_t capacity;

        struct CachedElem final {
            T cached_elem;
            Keyt key;

            CachedElem(T elem, Keyt key_) : cached_elem(elem), key(key_) {};
            CachedElem() = default;
        };

        using MapIterator = typename std::map<int, CachedElem>::iterator;
        std::map<int, CachedElem> cache;  

        std::unordered_map<Keyt, MapIterator> hash_map;
        std::unordered_map<Keyt, std::vector<int>> key_indexes;

        bool Full() const {

            return cache.size() == capacity;
        }

    public:
        PerfectCache(size_t cap, const std::vector<Keyt>& keys) : capacity(cap) { // fill map indexes
            
            int last_elem_index = keys.size() - 1;

            for (int index_keys = last_elem_index; index_keys >= 0; index_keys--) {
                key_indexes[keys[index_keys]].push_back(index_keys);
            }
        }

    #ifdef DEBUG_CACHE
        template<typename U, typename V> 
        friend void DebugPrintPerfectCache(const PerfectCache<U, V>& map);  
    #endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {

            auto index_it = key_indexes.find(key); 
            index_it->second.pop_back(); // pop_back from list indexes

            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) {
                if (index_it->second.empty()) {
                    key_indexes.erase(key);
                    return false;
                }

                if (Full()) {
                    Keyt key_to_pop = key;

                    auto remove_elem_it = cache.end();
                    remove_elem_it--;

                    auto cache_start_it = cache.begin();

                    if (cache_start_it->first < 0) {
                        key_to_pop = cache_start_it->second.key;
                        remove_elem_it = cache_start_it;
                    } else if (remove_elem_it->first > key_indexes[key].back()) {
                        key_to_pop = remove_elem_it->second.key;
                    } else {
                        return false;
                    }

                    cache.erase(remove_elem_it);
                    hash_map.erase(key_to_pop);    
                }

                int insert_elem_index = index_it->second.back();
                CachedElem new_elem(CachedElem(SlowGetPage(key), key));
                cache.emplace(insert_elem_index, new_elem);

                hash_map.emplace(key, cache.find(insert_elem_index));

            #ifdef DEBUG_CACHE
                DebugPrintPerfectCache(*this);
            #endif

                return false;
            }

            auto key_vector         = index_it->second;
            MapIterator cur_elem_it = hit->second;
            CachedElem cur_elem     = cur_elem_it->second;

            int new_index_elem = 0;

            if (!key_vector.empty()) {
                new_index_elem = key_vector.back();
            } else {
                new_index_elem = -cur_elem_it->first;
            }

            cache.erase(cur_elem_it);
            cache[new_index_elem] = cur_elem;
            hash_map[key] = cache.find(new_index_elem);

        #ifdef DEBUG_CACHE
            DebugPrintPerfectCache(*this);
        #endif

            return true;
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintPerfectCache(const PerfectCache<U, V>& perfect_cache) {

        std::cout << std::endl << "list: ";

        for (const auto& elem : perfect_cache.cache) {
            std::cout << elem.second.cached_elem << ":" << elem.first << " ";
        }

        std::cout << std::endl << "indexes_map: ";

        for (const auto& pair : perfect_cache.key_indexes) {
            std::cout << pair.first << ": ";

            for (const int& value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "  ";
        }

        std::cout << std::endl;
    }  
#endif

};

#endif // PERFECT_CACHE_HPP