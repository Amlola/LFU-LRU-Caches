#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <stdexcept>
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
        LRUCache(size_t cap) : capacity(0) {
            if (cap == 0) {
                throw std::invalid_argument("Invalid cache size");
            }

            capacity = cap;
        }

        const std::list<CachedElem>& GetCache() const {
            return cache;
        }   

        const std::unordered_map<Keyt, ListIterator>& GetHashMap() const {
            return hash_map;
        }

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {

            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) {
                if (Full()) {
                    hash_map.erase(cache.back().key);
                    cache.pop_back();
                }

                cache.push_front(CachedElem(SlowGetPage(key), key));
                hash_map.emplace(key, cache.begin());

            #ifdef DEBUG_CACHE
                DebugPrintLRUCache(*this);
            #endif

                return false;
            }

            const auto& find_page_it = hit->second;
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

    template<typename T, typename Keyt> 
    void DebugPrintLRUCache(const LRUCache<T, Keyt>& lru_cache) {

        std::cout << "\nhash_map: ";

        const auto& hash_map = lru_cache.GetHashMap();

        for (const auto&[key, value] : hash_map) {
            std::cout << key << ": ->" << value->cached_elem << "  ";
        }

        std::cout << "\nlist: ";

        const auto& cache = lru_cache.GetCache();

        for (const auto& elem : cache) {
            std::cout << elem.cached_elem << " ";
        }

        std::cout << "\n";
    }  
}

#endif // LRU_CACHE_HPP