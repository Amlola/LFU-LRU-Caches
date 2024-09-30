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

        struct CachedElem final {
            T cached_elem;
            Keyt key;

            CachedElem(T elem, Keyt key_) : cached_elem(elem), key(key_) {};
        };

        using ListIterator = typename std::list<CachedElem>::iterator;
        std::list<CachedElem> cache;  

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
                    hash_map.erase(cache.back().key);
                    cache.pop_back();
                }

                cache.push_front(CachedElem(SlowGetPage(key), key));
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
            
            return hash_map.at(key)->cached_elem;
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintLRUCache(const LRUCache<U, V>& lru_cache) {

        std::cout << std::endl << "hash_map: ";

        for (const auto& pair : lru_cache.hash_map) {
            std::cout << pair.first << ": ->" << pair.second->cached_elem << "  ";
        }

        std::cout << std::endl << "list: ";

        for (const auto& elem : lru_cache.cache) {
            std::cout << elem.cached_elem << " ";
        }

        std::cout << std::endl;
    }  
#endif

}

#endif // LRU_CACHE_HPP