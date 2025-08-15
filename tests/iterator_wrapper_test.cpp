#include <gtest/gtest.h>
#include "../include/iterator_wrapper.hpp"
#include "../include/range_iterator.hpp"

class IteratorWrapperTest {
public:
  static auto initialize_with_valid_iter() -> void {
    auto data = create_sample_data_();
    auto iterator = std::make_unique<RangeIterator<T>>(data.begin(), true);
    auto wrapper = IteratorWrapper(std::move(iterator));
    EXPECT_TRUE(wrapper != IteratorWrapper(nullptr));
  }

  static auto dereference_opertion() -> void {
    auto data = create_sample_data_();
    auto iterator = std::make_unique<RangeIterator<T>>(data.begin(), true);
    auto wrapper = IteratorWrapper(std::move(iterator));

    auto &row = *wrapper;
    EXPECT_EQ(row.first, 1);
    EXPECT_TRUE(data[1][0] == row.second[0].get());

    auto *ptr = wrapper.operator->();
    EXPECT_EQ(ptr->first, 1);
    EXPECT_TRUE(data[1][0] == ptr->second[0].get());
  }

  static auto increment_operation() -> void {
    auto data = create_sample_data_();
    auto iterator = std::make_unique<RangeIterator<T>>(data.begin(), true);
    auto wrapper = IteratorWrapper(std::move(iterator));

    while (wrapper.is_valid()) {
      std::cout << "entered" << std::endl;
      ++wrapper;
    }
  }

  static auto decrement_operation() -> void {
  }

private:
  using T = std::map<int64_t, std::vector<Data>>::const_iterator;
  static auto create_sample_data_() -> std::map<int64_t, std::vector<Data>> {
    return {
      {1, {Data("test1", "value1")}},
      {2, {Data("test2", "value2")}},
      {3, {Data("test3", "value3")}}
    };
  };
};

TEST(IteratorWrapperTest, InitializeWithValidIter) {
  IteratorWrapperTest::initialize_with_valid_iter();
}

TEST(IteratorWrapperTest, DereferenceOperationTest) {
  IteratorWrapperTest::dereference_opertion();
}
