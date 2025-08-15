#pragma once

#include "iterator.hpp"
#include <map>

template <std::input_iterator ConstIterator>
class RangeIterator : public Iterator {
  static_assert(std::is_same_v<ConstIterator, std::map<int64_t, std::vector<Data>>::const_iterator>,
                "ConstIterator must be a std::map const_iterator");

  TEST_FRIEND(RangeIteratorTest);

public:
  RangeIterator(ConstIterator it, bool ascending)
    : iter_(it), ascending_(ascending) {}

  auto operator!=(const Iterator &other) -> bool override {
    return !check_iter_equality_(other);
  }

  auto operator==(const Iterator &other) -> bool override {
    return check_iter_equality_(other);
  }

  auto operator++() -> Iterator & override {
    ascending_ ? ++iter_ : --iter_;
    return *this;
  }

  auto operator--() -> Iterator & override {
    ascending_ ? --iter_ : ++iter_;
    return *this;
  }

  auto operator*() const -> reference override {
    return get_current_data_().first;
  }

  auto operator->() const -> pointer override {
    return get_current_data_().second;
  }

private:
  ConstIterator iter_;
  bool ascending_;
  mutable Row current_;

  auto check_iter_equality_(const Iterator &other) const noexcept -> bool {
    const auto *other_it = dynamic_cast<const RangeIterator *>(&other);
    return other_it && iter_ == other_it->iter_;
  }

  auto get_current_data_() const -> std::pair<reference, pointer> {
    DataViewList views;

    for (const auto &data : iter_->second) {
      views.emplace_back(std::cref(data));
    }
    current_ = {iter_->first, std::move(views)};
    return {current_, &current_};
  };
};
