#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <list>
#include <iostream>
#include <cstddef>
#include <ostream>
#include <unordered_map>
#include <functional>

namespace Cache {

    template<typename T, typename Keyt = int> 
    class LRUCache final {

        size_t capacity;
        using ListIterator = typename std::list<Keyt>::iterator;
        std::list<T> cache;  

        std::unordered_map<Keyt, ListIterator> hash_table;

        bool Full() const {

            return cache.size() == capacity;
        }
  
    public:
        LRUCache(size_t cap) : capacity(cap) {};

#ifdef DEBUG_LRU_CACHE
        void DebugPrint() const {

            std::cout << "list: ";
            for (auto cache_it : cache) {
                std::cout << cache_it << " ";
            }

        std::cout << std::endl;
        }     
#endif

        template<typename SlowGetPage_t>
        bool LookupUpdate(Keyt key, SlowGetPage_t SlowGetPage) {

            auto hit = hash_table.find(key);
            if (hit == hash_table.end()) {
                if (Full()) {
                    hash_table.erase(cache.back());
                    cache.pop_back();
                }

                cache.push_front(SlowGetPage(key));
                hash_table[key] = cache.begin();

            #ifdef DEBUG_LRU_CACHE
                DebugPrint();
            #endif

                return false;
            }

            auto find_page_it = hit->second;
            if (find_page_it != cache.begin()) {
                cache.splice(cache.begin(), cache, find_page_it, std::next(find_page_it));
            }

            #ifdef DEBUG_LRU_CACHE
                DebugPrint();
            #endif

            return true;
        }
    };
}

#endif // LRU_CACHE_HPP