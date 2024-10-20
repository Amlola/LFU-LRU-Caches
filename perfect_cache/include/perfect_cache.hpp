#ifndef PERFECT_CACHE_HPP
#define PERFECT_CACHE_HPP

#include <iostream>
#include <cstddef>
#include <ostream>
#include <stdexcept>
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
        };

        using MapIterator = typename std::map<int, CachedElem>::iterator;
        std::map<int, CachedElem> cache;  

        std::unordered_map<Keyt, MapIterator> hash_map;
        std::unordered_map<Keyt, std::vector<int>> key_indexes;

        bool Full() const {

            return cache.size() == capacity;
        }

        void DebugPrintPerfectCache(std::ostream& os) const {

            os << "\nlist: ";

            for (const auto&[index, elem] : cache) {
                os << elem.cached_elem << ":" << index << " ";
            }

            os << "\nindexes_map: ";

            for (const auto&[key, indexes] : key_indexes) {
                os << key << ": ";

                for (const int& value : indexes) {
                    os << value << " ";
                }

                os << "  ";
            }

            os << "\n";
        }  

    public:
        PerfectCache(size_t cap, const std::vector<Keyt>& keys) : capacity(cap) { // fill map indexes

            int keys_size = keys.size();

            if (keys_size <= 0) {
                throw std::invalid_argument("No pages to cache");
            }
            
            const int& last_elem_index = keys_size - 1;

            for (int index_keys = last_elem_index; index_keys >= 0; index_keys--) {
                key_indexes[keys[index_keys]].push_back(index_keys);
            }
        }

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

                    auto remove_elem_it = std::prev(cache.end());
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

                const int& insert_elem_index = index_it->second.back();
                CachedElem new_elem(CachedElem(SlowGetPage(key), key));
                const auto& new_elem_it = cache.emplace(insert_elem_index, new_elem);

                hash_map.emplace(key, new_elem_it.first);

            #ifdef DEBUG_CACHE
                DebugPrintPerfectCache(std::cout);
            #endif

                return false;
            }

            const auto& key_vector  = index_it->second;
            MapIterator cur_elem_it = hit->second;
            CachedElem cur_elem     = cur_elem_it->second;

            int new_index_elem = 0;

            if (!key_vector.empty()) {
                new_index_elem = key_vector.back();
            } else {
                new_index_elem = -cur_elem_it->first;
            }

            cache.erase(cur_elem_it);
            auto new_elem_it = cache.emplace(new_index_elem, cur_elem);
            hash_map[key] = new_elem_it.first;

        #ifdef DEBUG_CACHE
            DebugPrintPerfectCache(std::cout);
        #endif

            return true;
        }
    };
}

#endif // PERFECT_CACHE_HPP