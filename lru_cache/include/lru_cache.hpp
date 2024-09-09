#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <unordered_map>

namespace Cache {

    template<typename T, typename Keyt = int> 
    class LRUCache final {

        size_t capacity;
        using ListIterator = typename std::list<Keyt>::iterator;
        std::list<T> cache;  

        std::unordered_map<Keyt, ListIterator> hash_map;

        bool Full() const {

            return cache.size() == capacity;
        }
  
    public:
        LRUCache(size_t cap) : capacity(cap) {};

    #ifdef DEBUG_CACHE
        template<typename U, typename V> 
        friend void DebugPrintLRUCache(const LRUCache<U, V>& map);  
    #endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {

            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) {
                if (Full()) {
                    hash_map.erase(cache.back());
                    cache.pop_back();
                }

                cache.push_front(SlowGetPage(key));
                hash_map[key] = cache.begin();

            #ifdef DEBUG_CACHE
                DebugPrintLRUCache(*this);
            #endif

                return false;
            }

            auto find_page_it = hit->second;
            if (find_page_it != cache.begin()) {
                cache.splice(cache.begin(), cache, find_page_it, std::next(find_page_it));
            }

        #ifdef DEBUG_CACHE
            DebugPrintLRUCache(*this);
        #endif

            return true;
        }

        T GetCachedValue(Keyt key) const {
            return *(hash_map.at(key));
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintLRUCache(const LRUCache<U, V>& map) {

    std::cout << std::endl << "hash_map: ";

    for (const auto& pair : map.hash_map) {
        std::cout << pair.first << ": ->" << *(pair.second) << "  ";
    }

    std::cout << "\n\n";
    }  
#endif

}

#endif // LRU_CACHE_HPP