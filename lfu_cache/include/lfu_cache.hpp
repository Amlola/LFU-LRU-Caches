#ifndef LFU_CACHE_HPP
#define LFU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <unordered_map>

namespace Cache {

    template<typename T, typename Keyt = int> 
    class LFUCache final {

        static constexpr size_t start_frequency = 1;

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
        LFUCache(size_t cap) : capacity(0), min_frequency(0) {
            if (cap == 0) {
                throw std::invalid_argument("Invalid cache size");
            }

            capacity = cap;
        };

        const std::unordered_map<size_t, std::list<CachedElem>>& GetCache() const {
            return cache;
        }   

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
                DebugPrintLFUCache(*this);
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
            DebugPrintLFUCache(*this);
        #endif

            return true;
        }
    };

    template<typename T, typename Keyt> 
    void DebugPrintLFUCache(const LFUCache<T, Keyt>& map) {

        std::cout << "\ncache:\n";

        const auto& cache = map.GetCache();

        for (const auto&[index, list] : cache) {
            std::cout << "freq " << index << ": ";

            for (const auto& elems : list) {
                std::cout << "(" << elems.key << ", " << elems.cached_elem << ", " << elems.frequency << ")  ";
            }

            std::cout << "\n";
        }

        std::cout << "\n";
    }  
}

#endif // LFU_CACHE_HPP