#pragma once

#include <concepts>
#include <iterator>
#include <map>
#include <string>

template <typename TValue, std::integral TId = std::size_t>
requires(!std::same_as<TValue, TId>) class id_bimap {
 private:
  std::map<TId, TValue> id_map;
  std::map<TValue, TId> value_map;
  TId next_id;

 public:
  id_bimap() noexcept : id_map(), value_map(), next_id() {}

  id_bimap(const id_bimap &other) noexcept = default;

  id_bimap(id_bimap &&other) noexcept = default;

  ~id_bimap() noexcept = default;

  using mapped_type = TValue;
  using key_type = TId;
  using iterator = typename std::map<TId, TValue>::const_iterator;

  // Iterators
  iterator begin() const noexcept { return id_map.begin(); }
  iterator end() const noexcept { return id_map.end(); }

  std::pair<iterator, bool> insert(const TValue &value) {
    if (value_map.contains(value)) {
      return std::pair{end(), false};
    }

    value_map.insert({value, next_id});
    auto it = id_map.insert({next_id, value}).first;

    ++next_id;

    return std::pair{it, true};
  }
};

template <typename TValue>
using kchar_id_bimap = id_bimap<TValue, char>;

using string_id_bimap = id_bimap<std::string>;
