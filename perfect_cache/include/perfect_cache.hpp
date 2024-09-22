#ifndef PERFECT_CACHE_HPP
#define PERFECT_CACHE_HPP

#include <iostream>
#include <list>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace Cache {

    template<typename T, typename Keyt = int> 
    class PerfectCache {

        size_t capacity;

        using ListIterator = typename std::list<std::pair<T, Keyt>>::iterator;
        std::list<std::pair<T, Keyt>> cache;  

        std::unordered_map<Keyt, ListIterator> hash_map;
        std::unordered_map<Keyt, std::list<size_t>> key_indexes;

        bool Full() const {

            return cache.size() == capacity;
        }

    public:
        PerfectCache(size_t cap, const std::vector<Keyt>& keys) : capacity(cap) { // fill map indexes
            
            size_t keys_size = keys.size();

            for (size_t i = 0; i < keys_size; i++) {
                key_indexes[keys[i]].push_back(i);
            }
        }

    #ifdef DEBUG_CACHE
        template<typename U, typename V> 
        friend void DebugPrintPerfectCache(const PerfectCache<U, V>& map);  
    #endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {

            auto index_it = key_indexes.find(key); 
            index_it->second.pop_front(); // pop_front from list indexes

            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) {
                if (index_it->second.empty()) {
                    key_indexes.erase(key);
                     return false;
                }

                if (Full()) {
                    auto max_list_it = key_indexes.find(key);

                    Keyt key_to_pop = key;

                    for (auto& pair : cache) {
                        auto cur_it_indexes = key_indexes.find(pair.second);

                        if (cur_it_indexes->second.empty()) {
                            key_to_pop = cur_it_indexes->first;
                            key_indexes.erase(key_to_pop);
                            break;
                        }

                        if (cur_it_indexes->second.front() >= max_list_it->second.front()) {
                            max_list_it = cur_it_indexes;
                            key_to_pop  = cur_it_indexes->first;
                        }
                    }

                    if (key_to_pop == key)
                        return false;

                    cache.erase(hash_map[key_to_pop]);
                    hash_map.erase(key_to_pop);   
                }

                cache.push_front({SlowGetPage(key), key});
                hash_map[key] = cache.begin();

            #ifdef DEBUG_CACHE
                DebugPrintPerfectCache(*this);
            #endif

                return false;
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

        for (const auto& pair : perfect_cache.hash_map) {
            std::cout << pair.first << ": ->" << (*(pair.second)).first << "  ";
        }

        std::cout << std::endl << "list: ";

        for (const auto& elem : perfect_cache.cache) {
            std::cout << elem.first << " ";
        }

        std::cout << std::endl << "indexes_map: ";

        for (const auto& pair : perfect_cache.key_indexes) {
            std::cout << pair.first << ": ";

            for (const size_t& value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "  ";
        }

        std::cout << std::endl;
    }  
#endif

};

#endif // PERFECT_CACHE_HPP