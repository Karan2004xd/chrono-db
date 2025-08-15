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

  virtual ~Iterator() = default;
  virtual auto operator!=(const Iterator &other) -> bool = 0;
  virtual auto operator==(const Iterator &other) -> bool = 0;

  virtual auto operator++() -> Iterator & = 0;
  virtual auto operator--() -> Iterator & = 0;

  virtual auto operator*() const -> reference = 0;
};
