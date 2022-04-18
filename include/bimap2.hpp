#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>  // For std::ptrdiff_t
#include <iterator>
#include <iterator>  // For std::forward_iterator_tag
#include <map>
#include <memory>
#include <ranges>
#include <string>

template <typename TValue, std::integral TId = std::size_t>
requires(!std::same_as<TValue, TId>) class id_bimap {
 public:
  // Iterator
  struct const_iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using vector_pointer = typename std::vector<TValue>::const_iterator;
    using value_type = std::pair<const TId&, const TValue&>;
    using pointer = value_type*;
    using reference = value_type&;

    const_iterator(vector_pointer ptr, vector_pointer begin)
        : m_ptr(ptr), m_begin(begin) {
      set_current();
    }

    const_iterator(const const_iterator& other)
        : m_ptr(other.m_ptr), m_begin(other.m_begin) {
      set_current();
    }

    const reference operator*() const { return *m_current; }

    const pointer operator->() { return &*m_current; }

    const_iterator& operator++() {
      m_ptr++;
      set_current();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator& a, const const_iterator& b) {
      return a.m_ptr == b.m_ptr;
    };

   private:
    vector_pointer m_ptr;
    vector_pointer m_begin;
    TId m_current_id;
    std::optional<value_type> m_current;

    void set_current() {
      m_current_id = std::distance(m_begin, m_ptr);
      m_current.emplace(m_current_id, *m_ptr);
    }
  };

 private:
  std::vector<TValue> id_map;

  void delete_id_and_shift(const TId& id) { id_map.erase(id_map.begin() + id); }

  const_iterator iterator_for_id(const TId& id) const {
    return const_iterator(id_map.begin() + id, id_map.begin());
  }

  std::optional<TId> id_for_value(const TValue& value_to_search) const {
    for (const auto& [id, value] : *this) {
      if (value == value_to_search) {
        return id;
      }
    }

    return {};
  }

 public:
  id_bimap() noexcept : id_map() {}

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

  // Iterators
  const_iterator begin() const noexcept {
    return const_iterator(id_map.begin(), id_map.begin());
  }
  const_iterator end() const noexcept {
    return const_iterator(id_map.end(), id_map.begin());
  }

  // Modify
  std::pair<const_iterator, bool> insert(const TValue& value) {
    if (const auto& id = id_for_value(value); id) {
      auto id_it = iterator_for_id(*id);
      return std::pair{id_it, false};
    }

    id_map.push_back(value);
    auto it = iterator_for_id(id_map.size() - 1);

    return std::pair{it, true};
  }

  template <typename... Args>
  std::pair<const_iterator, bool> emplace(Args&&... args) {
    // ?: Do we need to check whether the constructed value is in the map?
    id_map.emplace_back(std::forward<Args>(args)...);
    auto it = iterator_for_id(id_map.size() - 1);

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

  std::size_t erase(const TId& id) {
    if (id >= id_map.size()) {
      return 0;
    }

    delete_id_and_shift(id);

    return 1;
  }

  template <typename UnaryPredicate>
  std::size_t delete_all(UnaryPredicate predicate) {
    std::size_t deleted_count = 0;
    const auto size = id_map.size();

    for (TId i = 0; i < size; ++i) {
      if (predicate(id_map[i - deleted_count])) {
        delete_id_and_shift(i - deleted_count);
        ++deleted_count;
      }
    }

    return deleted_count;
  }

  // Query
  std::size_t size() const noexcept { return id_map.size(); }

  bool empty() const noexcept { return id_map.empty(); }

  TId operator[](const TValue& value) const {
    const auto& id = id_for_value(value);
    if (!id) {
      throw std::domain_error{"Value not found in id_map."};
    }

    return *id;
  }

  const TValue& operator[](const TId& id) const { return id_map.at(id); }

  const_iterator find(const TValue& value) const {
    const auto& id = id_for_value(value);
    if (!id) {
      return end();
    }
    return iterator_for_id(*id);
  }

  template <typename... Args>
  const_iterator find(Args... args) const {
    const TValue value = TValue(args...);
    return find(value);
  }

  template <typename UnaryPredicate>
  const_iterator find_if(UnaryPredicate predicate) const {
    for (const auto& [id, value] : *this) {
      if (predicate(value)) {
        return iterator_for_id(id);
      }
    }
    return end();
  }
};

template <typename TValue>
using kchar_id_bimap = id_bimap<TValue, char>;

using string_id_bimap = id_bimap<std::string>;
