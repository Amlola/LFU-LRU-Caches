#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <unordered_map>

namespace Cache {

    template<typename T, typename Keyt = int> 
    class LFUCache {

        size_t capacity;
        using ListIterator = typename std::list<Keyt>::iterator;
        std::list<T> cache;  

        std::unordered_map<Keyt, ListIterator> hash_map;

        bool Full() const {

            return cache.size() == capacity;
        }

        public:
            LFUCache(size_t cap) : capacity(cap) {}; // READ: about this

        #ifdef DEBUG_CACHE
            template<typename U, typename V> 
            friend void DebugPrintLFUCache(const LFUCache<U, V>& map);  
        #endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {
            // TODO: it's time to do this
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintLFUCache(const LFUCache<U, V>& map) {

    std::cout << std::endl << "hash_map: ";

    for (const auto& pair : map.hash_map) {
        std::cout << pair.first << ": ->" << *(pair.second) << "  ";
    }

    std::cout << "\n\n";
    }  
#endif
}

#endif // LFU_CACHE_HPP