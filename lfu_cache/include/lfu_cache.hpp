#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <unordered_map>

const size_t start_frequency = 1;

namespace Cache {

    template<typename T, typename Keyt = int> 
    class LFUCache final {

        size_t capacity;
        size_t min_frequency;

        struct CachedElem final {
            T cached_elem;
            Keyt key;
            size_t frequency;

            CachedElem(T elem, Keyt key_) : cached_elem(elem), key(key_), frequency(start_frequency) {};
        };
        
        using ListIterator = typename std::list<CachedElem>::iterator;
        
        std::unordered_map<size_t, std::list<CachedElem>> cache;  
        std::unordered_map<Keyt, ListIterator> hash_map;

        bool Full() const {

            return hash_map.size() == capacity;
        }

        public:
            LFUCache(size_t cap) : capacity(cap), min_frequency(0) {};

        #ifdef DEBUG_CACHE
            template<typename U, typename V> 
            friend void DebugPrintLFUCache(const LFUCache<U, V>& map);  
        #endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {
            
            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) {
                if (Full()) {

                    auto& min_freq_elems = cache[min_frequency];
                    hash_map.erase(min_freq_elems.back().key);
                    min_freq_elems.pop_back(); // FIFO

                    if (min_freq_elems.empty()) {
                        cache.erase(min_frequency);
                    }
                }

                CachedElem new_elem(SlowGetPage(key), key);

                cache[start_frequency].push_front(new_elem);
                min_frequency = start_frequency;

                hash_map[key] = cache[start_frequency].begin();

            #ifdef DEBUG_CACHE
                DebugPrintLFUCache(*this);
            #endif

                return false;
            }

            CachedElem find_elem = *(hit->second);
            cache[find_elem.frequency].erase(hit->second);

            if (cache[find_elem.frequency].empty() && find_elem.frequency == min_frequency) {
                cache.erase(find_elem.frequency);
                min_frequency++;
            }

            find_elem.frequency++;
            cache[find_elem.frequency].push_front(find_elem);

            hash_map[key] = cache[find_elem.frequency].begin(); // FIFO

        #ifdef DEBUG_CACHE
            DebugPrintLFUCache(*this);
        #endif

            return true;
        }
    };

#ifdef DEBUG_CACHE
    template<typename U, typename V> 
    void DebugPrintLFUCache(const LFUCache<U, V>& map) {

    std::cout << std::endl << "cache:" << std::endl;

    for (const auto& pair : map.cache) {
        std::cout << "freq " << pair.first << ": ";

        for (const auto& elems : pair.second) {
            std::cout << "(" << elems.key << ", " << elems.cached_elem << ", " << elems.frequency << ")  ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
    }  
#endif

}

#endif // LFU_CACHE_HPP