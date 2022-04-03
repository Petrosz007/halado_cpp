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

  /**
   * @brief Deletes `id` and shifts every ID greather than this down by one. The
   * `id` must exist.
   *
   * @param id Id to delete
   */
  void delete_id_and_shift(const TId& id) {
    auto value = id_map.at(id);

    id_map.erase(id);
    value_map.erase(value);

    for (auto i = id + 1; i < next_id; ++i) {
      auto val = id_map.at(i);

      id_map.erase(i);
      value_map.erase(val);

      id_map.insert({i - 1, val});
      value_map.insert({val, i - 1});
    }

    --next_id;
  }

 public:
  id_bimap() noexcept : id_map(), value_map(), next_id() {}

  id_bimap(const id_bimap& other) noexcept = default;

  id_bimap(id_bimap&& other) noexcept = default;

  id_bimap(std::initializer_list<TValue> init) noexcept : id_bimap() {
    for (auto value : init) {
      insert(value);
    }
  }

  ~id_bimap() noexcept = default;

  using mapped_type = TValue;
  using key_type = TId;
  using const_iterator = typename std::map<TId, TValue>::const_iterator;

  // Iterators
  const_iterator begin() const noexcept { return id_map.begin(); }
  const_iterator end() const noexcept { return id_map.end(); }

  // Modify
  std::pair<const_iterator, bool> insert(const TValue& value) {
    if (value_map.contains(value)) {
      auto id = value_map.at(value);
      auto id_it = id_map.find(id);
      return std::pair{id_it, false};
    }

    value_map.insert({value, next_id});
    auto it = id_map.insert({next_id, value}).first;

    ++next_id;

    return std::pair{it, true};
  }

  void clear() noexcept {
    value_map.clear();
    id_map.clear();
  }

  std::size_t erase(const TValue& value) {
    if (!value_map.contains(value)) {
      return 0;
    }

    auto id = value_map.at(value);
    delete_id_and_shift(id);

    return 1;
  }

  std::size_t erase(const TId& id) {
    if (!id_map.contains(id)) {
      return 0;
    }

    delete_id_and_shift(id);

    return 1;
  }

  // Query
  std::size_t size() const noexcept { return value_map.size(); }

  bool empty() const noexcept { return value_map.empty(); }

  const TId& operator[](const TValue& value) const {
    if (!value_map.contains(value)) {
      throw std::domain_error{"Value not found in id_map."};
    }

    return value_map.at(value);
  }

  const TValue& operator[](const TId& id) const { return id_map.at(id); }

  const_iterator find(const TValue& value) const {
    if (!value_map.contains(value)) {
      return end();
    }
    auto id = value_map.at(value);
    return id_map.find(id);
  }

  const_iterator find(const TId& id) const { return id_map.find(id); }
};

template <typename TValue>
using kchar_id_bimap = id_bimap<TValue, char>;

using string_id_bimap = id_bimap<std::string>;
