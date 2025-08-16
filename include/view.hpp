#pragma once

#include "range_iterator.hpp"

template <typename IteratorType>
class View {
  TEST_FRIEND(ViewTest);

  static_assert(
    std::is_base_of_v<Iterator, IteratorType>,
    "Iterator Type must be derived from the Iterator class"
  );

public:
  View() = default;

  View(IteratorType begin_it, IteratorType end_it)
    : begin_it_(begin_it), end_it_(end_it) {}

  View(const View &other) 
    : begin_it_(other.begin_it_), end_it_(other.end_it_) {}

  auto operator=(const View &other) {
    begin_it_ = other.begin_it_;
    end_it_ = other.end_it_;
  }
  
  auto operator=(View &&other) noexcept {
    begin_it_ = std::move(other.begin_it_);
    end_it_ = std::move(other.end_it_);

    begin_it_ = IteratorType{};
    end_it_ = IteratorType{};
  }

  View(View &&other) noexcept {
    begin_it_ = std::move(other.begin_it_);
    end_it_ = std::move(other.end_it_);

    begin_it_ = IteratorType{};
    end_it_ = IteratorType{};
  }

  auto begin() const -> IteratorType {
    return begin_it_;
  }

  auto end() const -> IteratorType {
    return end_it_;
  }

private:
  IteratorType begin_it_ {};
  IteratorType end_it_ {};
};

template <typename RawIterator>
inline auto make_data_view(RawIterator begin,
                           RawIterator end,
                           bool ascending) -> View<RangeIterator<RawIterator>> {

  using CustomIterator = RangeIterator<RawIterator>;

  return View<CustomIterator> {
    CustomIterator(begin, ascending),
    CustomIterator(end, ascending)
  };
}
