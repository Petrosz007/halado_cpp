#pragma once

#include <algorithm>
#include <concepts>
#include <iterator>
#include <map>
#include <memory>
#include <ranges>
#include <string>

template <typename TValue, std::integral TId = std::size_t>
requires(!std::same_as<TValue, TId>) class id_bimap {
 private:
  std::map<TId, TValue> id_map;
  TId next_id;

  /**
   * @brief Deletes `id` and shifts every ID greather than this down by one. The
   * `id` must exist.
   *
   * @param id Id to delete
   */
  void delete_id_and_shift(const TId id) {
    id_map.erase(id);

    for (auto i = id + 1; i < next_id; ++i) {
      auto node = id_map.extract(i);
      --node.key();

      id_map.insert(std::move(node));
    }

    --next_id;
  }

  std::optional<TId> id_for_value(const TValue& value_to_search) const {
    for (const auto& [key, value] : id_map) {
      if (value == value_to_search) {
        return key;
      }
    }

    return {};
  }

 public:
  id_bimap() noexcept : id_map(), next_id() {}

  id_bimap(const id_bimap& other) noexcept = default;

  id_bimap(id_bimap&& other) noexcept = default;

  explicit id_bimap(std::initializer_list<TValue> init) noexcept : id_bimap() {
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
    const auto& id = id_for_value(value);
    if (id) {
      auto id_it = id_map.find(*id);
      return std::pair{id_it, false};
    }

    auto it = id_map.insert({next_id, value}).first;

    ++next_id;

    return std::pair{it, true};
  }

  std::pair<const_iterator, bool> emplace(TValue value) {
    const auto& id = id_for_value(value);
    if (id) {
      auto id_it = id_map.find(*id);
      return std::pair{id_it, false};
    }

    auto it = id_map.insert({next_id, std::move(value)}).first;

    ++next_id;

    return std::pair{it, true};
  }

  void clear() noexcept { id_map.clear(); }

  std::size_t erase(const TValue& value) {
    const auto& id = id_for_value(value);
    if (!id) {
      return 0;
    }

    delete_id_and_shift(*id);

    return 1;
  }

  std::size_t erase(const TId id) {
    if (!id_map.contains(id)) {
      return 0;
    }

    delete_id_and_shift(id);

    return 1;
  }

  template <typename UnaryPredicate>
  std::size_t delete_all(UnaryPredicate predicate) {
    std::size_t deleted_count = 0;
    for (const auto& [id, value] : id_map) {
      if (predicate(value)) {
        delete_id_and_shift(id - deleted_count);
        ++deleted_count;
      }
    }

    return deleted_count;
  }

  // Query
  std::size_t size() const noexcept { return id_map.size(); }

  bool empty() const noexcept { return id_map.empty(); }

  const TId operator[](const TValue& value) const {
    const auto& id = id_for_value(value);
    if (!id) {
      throw std::domain_error{"Value not found in id_map."};
    }

    return *id;
  }

  const TValue& operator[](const TId id) const { return id_map.at(id); }

  const_iterator find(const TValue& value) const {
    const auto& id = id_for_value(value);
    if (!id) {
      return end();
    }
    return id_map.find(*id);
  }

  const_iterator find(const TId id) const { return id_map.find(id); }

  template <typename UnaryPredicate>
  const_iterator find_if(UnaryPredicate predicate) const {
    for (const auto& [key, value] : id_map) {
      if (predicate(value)) {
        return id_map.find(key);
      }
    }
    return end();
  }
};

template <typename TValue>
using kchar_id_bimap = id_bimap<TValue, char>;

using string_id_bimap = id_bimap<std::string>;
