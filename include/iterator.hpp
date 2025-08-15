#pragma once

#include "data.hpp"
#include <vector>

struct Iterator {
  using DataView = std::reference_wrapper<const Data>;
  using DataViewList = std::vector<DataView>;

  using iterator_category = std::input_iterator_tag;
  using Row = std::pair<int64_t, DataViewList>;

  using pointer = const Row *;
  using reference = const Row &;

  Iterator(const Iterator &) = default;
  Iterator(Iterator &&) noexcept = default;

  auto operator=(const Iterator &) -> Iterator & = default;
  auto operator=(Iterator &&) noexcept -> Iterator & = default;

  virtual ~Iterator() = default;
  virtual auto operator!=(const Iterator &other) -> bool = 0;
  virtual auto operator==(const Iterator &other) -> bool = 0;

  virtual auto operator++() -> Iterator & = 0;
  virtual auto operator--() -> Iterator & = 0;

  virtual auto operator*() const -> reference = 0;
  virtual auto operator->() const -> pointer = 0;
};
