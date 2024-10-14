#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <ostream>
#include <unordered_map>
#include "unified_cache.hpp"

namespace Cache {

    template<typename T, typename Keyt = int> 
    class LFUCache final : public CacheBase<LFUCache<T, Keyt>, T, Keyt> {

        static constexpr size_t start_frequency = 1;

        size_t min_frequency = 0;

        using CacheBase<LFUCache<T, Keyt>, T, Keyt>::capacity;

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
        
        void DebugPrintLFUCache(std::ostream& os) const {

            os << "\ncache:\n";

            for (const auto&[index, list] : cache) {
                os << "freq " << index << ": ";

                for (const auto& elems : list) {
                    os << "(" << elems.key << ", " << elems.cached_elem << ", " << elems.frequency << ")  ";
                }

                os << "\n";
            }

            os << "\n";
        }   

    public:
        LFUCache(size_t cap) : CacheBase<LFUCache<T, Keyt>, T, Keyt>(cap) {}

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {
            
            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) {
                if (Full()) {

                    auto& min_freq_elems = cache[min_frequency];
                    hash_map.erase(min_freq_elems.back().key);
                    min_freq_elems.pop_back();

                    if (min_freq_elems.empty()) {
                        cache.erase(min_frequency);
                    }
                }

                CachedElem new_elem(SlowGetPage(key), key);

                auto& start_freq_list = cache[start_frequency];
                start_freq_list.push_front(new_elem);
                min_frequency = start_frequency;

                hash_map.emplace(key, start_freq_list.begin());

            #ifdef DEBUG_CACHE
                DebugPrintLFUCache(std::cout);
            #endif

                return false;
            }

            ListIterator find_cached_elem_list_it = hit->second;

            CachedElem find_elem = *(find_cached_elem_list_it);        
            cache[find_elem.frequency].erase(find_cached_elem_list_it); 

            if (cache[find_elem.frequency].empty() && find_elem.frequency == min_frequency) {
                cache.erase(find_elem.frequency);
                min_frequency++;
            }

            find_elem.frequency++;
            auto& new_freq_list = cache[find_elem.frequency];
            new_freq_list.push_front(find_elem);

            hash_map[key] = new_freq_list.begin(); // FIFO

        #ifdef DEBUG_CACHE
            DebugPrintLFUCache(std::cout);
        #endif

            return true;
        }
    }; 
}

#endif // LFU_CACHE_HPP