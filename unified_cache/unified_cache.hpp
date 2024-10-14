#pragma once

#include <cstddef>
#include <stdexcept>

namespace Cache {

    template <typename CacheT, typename T, typename Keyt = int>
    class CacheBase {

        static constexpr size_t max_size_of_cache = 1e10;

    protected:

        size_t capacity;
        
        CacheBase(size_t cap) : capacity(cap) {

            if (cap > max_size_of_cache) {
                throw std::invalid_argument("Invalid cache size");
            }
        }
    
    public:

        template<typename SlowGetPage_t>
        bool Update(Keyt key, SlowGetPage_t SlowGetPage) {

            return static_cast<CacheT*>(this)->LookupUpdate(key, SlowGetPage);
        }
    };
}