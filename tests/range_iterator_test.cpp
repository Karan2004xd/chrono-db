#include <gtest/gtest.h>
#include "../include/range_iterator.hpp"

class RangeIteratorTest {
public:
  static auto single_element_range() -> void {
    auto data = std::map<int64_t, std::vector<Data>>{
      {1, {Data("test", "hello")}},
    };

    auto start = RangeIterator<T>(data.lower_bound(1), true);
    auto end = RangeIterator<T>(data.upper_bound(1), true);
    auto it = start;

    EXPECT_TRUE(it != end);
    EXPECT_TRUE(++it == end);
    EXPECT_TRUE(--it == start);

    EXPECT_EQ(std::distance(start.iter_, end.iter_), 1);

    auto value = *it;
    EXPECT_EQ(value.first, 1);
    EXPECT_TRUE((Data("test", "hello")) == value.second[0]);
  }

  static auto multiple_element_range_asc() -> void {
    auto timestamps = std::vector<int64_t> {
      1001, 1002, 1003, 1004
    };
    auto n = timestamps.size();

    auto data = std::map<int64_t, std::vector<Data>>{
      {timestamps[0], {Data("test", "hello")}},
      {timestamps[1], {Data("test1", "hello1")}},
      {timestamps[2], {Data("test2", "hello2")}},
      {timestamps[3], {Data("test3", "hello3")}}
    };

    auto start = RangeIterator<T>(data.lower_bound(timestamps[0]), true);
    auto end = RangeIterator<T>(data.upper_bound(timestamps[n - 1]), true);
    auto it = start;
    auto i = 0;

    while (it != end) {
      auto value = *it;
      EXPECT_EQ(value.first, timestamps[i]);
      ++it, ++i;
    }
    EXPECT_TRUE(it == end);
  }

  static auto multiple_element_range_dsc() -> void {
    auto timestamps = std::vector<int64_t> {
      1001, 1002, 1003, 1004
    };
    auto n = timestamps.size();

    auto data = std::map<int64_t, std::vector<Data>>{
      {timestamps[0], {Data("test", "hello")}},
      {timestamps[1], {Data("test1", "hello1")}},
      {timestamps[2], {Data("test2", "hello2")}},
      {timestamps[3], {Data("test3", "hello3")}}
    };

    auto start = RangeIterator<T>(data.upper_bound(timestamps[n - 1]), false);
    auto end = RangeIterator<T>(data.lower_bound(timestamps[0]), false);

    auto it = end;
    auto i = 0;

    while (it != start) {
      auto value = *it;
      EXPECT_EQ(value.first, timestamps[i]);
      --it, ++i;
    }
    EXPECT_TRUE(it == start);
  }

  static auto dereferencing_and_member_access() -> void {
    auto data = std::map<int64_t, std::vector<Data>>{
      {1, {Data("test", "hello")}},
    };

    auto start = RangeIterator<T>(data.lower_bound(1), true);
    auto end = RangeIterator<T>(data.upper_bound(1), true);
    auto it = start;

    auto ref_val = *it;
    EXPECT_EQ(ref_val.first, 1);
    EXPECT_TRUE((Data("test", "hello")) == ref_val.second[0]);

    auto ptr_val = it;
    EXPECT_EQ(ptr_val->first, 1);
    EXPECT_TRUE((Data("test", "hello")) == ptr_val->second[0]);
  }

private:
  using T = std::map<int64_t, std::vector<Data>>::const_iterator;
};

TEST(RangeIteratorTest, SingleElementRangeTest) {
  RangeIteratorTest::single_element_range();
}

TEST(RangeIteratorTest, MultipleElementAscendingTest) {
  RangeIteratorTest::multiple_element_range_asc();
}

TEST(RangeIteratorTest, MultipleElementDescendingTest) {
  RangeIteratorTest::multiple_element_range_dsc();
}

TEST(RangeIteratorTest, DereferencingMemberAccessTest) {
  RangeIteratorTest::dereferencing_and_member_access();
}
