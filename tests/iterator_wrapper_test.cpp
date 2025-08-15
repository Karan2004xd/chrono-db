#include <gtest/gtest.h>
#include "../include/iterator_wrapper.hpp"
#include "../include/range_iterator.hpp"

class IteratorWrapperTest {
public:
  static auto initialize_and_deref() -> void {
    auto data = std::map<int64_t, std::vector<Data>>{
      {1, {Data("test", "hello")}},
    };

    auto iterator = std::make_unique<RangeIterator<T>>(data.begin(), true);
    auto test_iterator = std::make_unique<RangeIterator<T>>(data.begin(), true);
    auto wrapper = IteratorWrapper(std::move(iterator));
  }

private:
  using T = std::map<int64_t, std::vector<Data>>::const_iterator;
};

TEST(IteratorWrapperTest, InitializeAndDref) {
  IteratorWrapperTest::initialize_and_deref();
}
