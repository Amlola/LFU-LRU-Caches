#ifndef PERFECT_CACHE_HPP
#define PERFECT_CACHE_HPP

#include <iostream>
#include <list>
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

                    auto it = cache.end();
                    it--;

                    if (cache.begin()->first < 0) {
                        key_to_pop = cache.begin()->second.key;
                        it = cache.begin();
                    } else if (key_indexes[it->second.key].empty()) {
                        key_to_pop = it->second.key;
                        key_indexes.erase(key_to_pop);
                    } else if (it->first >= key_indexes[key].back()) {
                        key_to_pop = it->second.key;
                    } else {
                        return false;
                    }

                    if (key_to_pop == key) {
                        return false;
                    }

                    cache.erase(it);
                    hash_map.erase(key_to_pop);    
                }

                int key_insert_place = index_it->second.back();
                CachedElem new_elem(CachedElem(SlowGetPage(key), key));
                cache.emplace(key_insert_place, new_elem);

                hash_map[key] = cache.find(key_insert_place);

            #ifdef DEBUG_CACHE
                DebugPrintPerfectCache(*this);
            #endif

                return false;
            }

            CachedElem cur_elem = hit->second->second;

            if (!index_it->second.empty()) {
                cache.erase(hit->second);
                cache[index_it->second.back()] = cur_elem;
                hash_map[key] = cache.find(index_it->second.back());
            } else {
                int place = -hit->second->first;
                cache.erase(hit->second);
                cache[place] = cur_elem;
                hash_map[key] = cache.find(place);
            }

        #ifdef DEBUG_CACHE
            DebugPrintPerfectCache(*this);
        #endif

            return true;
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintPerfectCache(const PerfectCache<U, V>& perfect_cache) {

        std::cout << std::endl << "hash_map: ";

        // for (const auto& pair : perfect_cache.hash_map) {
        //     std::cout << pair.first << ": ->" << pair.second->cached_elem << "  ";
        // }

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