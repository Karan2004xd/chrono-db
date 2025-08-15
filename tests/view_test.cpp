#include <gtest/gtest.h>
#include "../include/view.hpp"

class ViewTest {
public:
  static auto begin_and_iterators_test() -> void {
    auto timestamps = std::vector<int64_t>{1, 2};
    auto data = RangeData {
      {timestamps[0], {Data("test", "value")}},
      {timestamps[1], {Data("test1", 2)}},
    };

    auto view = make_data_view<RangeDataIter>(
      data.begin(), data.end(), true
    );
    auto i = 0;

    for (const auto &[ts, data_list] : view) {
      EXPECT_EQ(timestamps[i], ts);
      for (const auto &data_obj : data_list) {
        EXPECT_TRUE(data[ts][0] == data_obj.get());
      }
      i++;
    }
  }
};

TEST(ViewTest, TestFunc) {
  ViewTest::begin_and_iterators_test();
}
