#pragma once

#include <map>
#include <string>

template<typename TValue, typename TId = std::size_t>
class id_bimap
{
private:
    std::map<TId, TValue> id_map;
    std::map<TValue, TId> value_map;

public:
    id_bimap() : id_map(), value_map()
    {
        static_assert(std::is_integral<TId>::value);
        static_assert(!std::is_same<TId, TValue>::value);
    }

    // ?: Is this correct?
    id_bimap(const id_bimap& other) noexcept :
        id_map(other.id_map),
        value_map(other.value_map)
    {}


    // ?: Is this correct?
    id_bimap(const id_bimap&& other) noexcept :
        id_map(std::move(other.id_map)),
        value_map(std::move(other.value_map))
    {}

    ~id_bimap() noexcept
    {
        // TODO: What should we destruct here?
    }

    using mapped_type = TValue;
    using key_type = TId;
};


template<typename TValue>
using kchar_id_bimap = id_bimap<TValue, char>;

using string_id_bimap = id_bimap<std::string>;
