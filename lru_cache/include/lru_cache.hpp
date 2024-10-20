#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <ostream>
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

        void DebugPrintLRUCache(std::ostream& os) const {

            os << "\nhash_map: ";

            for (const auto&[key, value] : hash_map) {
                os << key << ": ->" << value->cached_elem << "  ";
            }

            os << "\nlist: ";

            for (const auto& elem : cache) {
                os << elem.cached_elem << " ";
            }

            os << "\n\n";
        }  
  
    public:
        LRUCache(size_t cap) : capacity(cap) {}

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
                DebugPrintLRUCache(std::cout);
            #endif

                return false;
            }

            const auto& find_page_it = hit->second;
            if (find_page_it != cache.begin()) {
                cache.splice(cache.begin(), cache, find_page_it, std::next(find_page_it));
            }

        #ifdef DEBUG_CACHE
            DebugPrintLRUCache(std::cout);
        #endif

            return true;
        }

        T GetCachedValue(Keyt key) const {
            
            return hash_map.at(key)->cached_elem;
        }
    };
}

#endif // LRU_CACHE_HPP