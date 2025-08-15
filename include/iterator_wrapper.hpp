#pragma once

#include "iterator.hpp"

class IteratorWrapper {
  TEST_FRIEND(IteratorWrapperTest);

public:
  using DataView = std::reference_wrapper<const Data>;
  using DataViewList = std::vector<DataView>;

  using Row = std::pair<int64_t, DataViewList>;

  explicit IteratorWrapper(Iterator &&begin, Iterator &&end);

  auto operator*() const -> const Row &;
  auto operator->() const -> const Row *;

  auto operator++() -> IteratorWrapper &;
  auto operator--() -> IteratorWrapper &;

  auto operator!=(const IteratorWrapper &other) const -> bool;
  auto operator==(const IteratorWrapper &other) const -> bool;

  auto is_valid() const noexcept -> bool;

private:
  std::unique_ptr<Iterator> begin_, end_;
  std::unique_ptr<Iterator> iter_;

  auto check_equality_base_(const IteratorWrapper &other) const noexcept -> bool;
};
