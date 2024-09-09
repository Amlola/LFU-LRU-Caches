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
        using ListIterator = typename std::list<Keyt>::iterator;
        std::list<T> cache;  

        std::unordered_map<Keyt, ListIterator> hash_map;

        bool Full() const {

            return cache.size() == capacity;
        }

        public:
            PerfectCache(size_t cap) : capacity(cap) {}; // TODO: std::vector

        #ifdef DEBUG_CACHE
            template<typename U, typename V> 
            friend void DebugPrintPerfectCache(const PerfectCache<U, V>& map);  
        #endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {
            // TODO: it's time to do this
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintPerfectCache(const LFUCache<U, V>& map) {

    std::cout << std::endl << "hash_map: ";

    for (const auto& pair : map.hash_map) {
        std::cout << pair.first << ": ->" << *(pair.second) << "  ";
    }

    std::cout << "\n\n";
    }  
#endif

};

#endif // PERFECT_CACHE_HPP