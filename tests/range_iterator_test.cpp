#include <gtest/gtest.h>
#include <map>
#include "../include/range_iterator.hpp"

class RangeIteratorTest {
public:
  static auto single_element_range() -> void {
    auto data = std::map<int64_t, std::vector<Data>>{
      {1, {Data("test", "hello")}},
      {2, {Data("test", "hello")}}
    };

    auto start = RangeIterator<T>(data.lower_bound(1), true);
    auto end = RangeIterator<T>(data.upper_bound(2), true);
  }

private:
  using T = std::map<int64_t, std::vector<Data>>::const_iterator;
};

TEST(RangeIteratorTest, SingleElementRangeTest) {
  RangeIteratorTest::single_element_range();
}
