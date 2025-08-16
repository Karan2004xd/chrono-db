#include <gtest/gtest.h>
#include "../include/view.hpp"

class ViewTest {
public:
  static auto iterators() -> void {
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

  static auto semantics() -> void {
    auto data = RangeData {
      {0, {Data("test", "value")}},
      {1, {Data("test1", "value2")}}
    };

    auto view1 = make_data_view<RangeDataIter>(
      data.begin(), data.end(), true
    );

    // Copy Semantics
    auto view2 = view1;
    check_view_equality_(view1, view2);

    auto view3 = make_data_view<RangeDataIter>(
      data.begin(), data.end(), true
    );
    check_view_equality_(view1, view3);

    // Move Semantics
    view2 = make_data_view<RangeDataIter>(
      data.begin(), data.end(), true
    );
    view3 = std::move(view1);

    check_view_equality_(view3, view2);
    check_view_equality_(view1, View<RangeIterator<RangeDataIter>>());
  }

private:
  template <typename T>
  static auto check_view_equality_(const View<T> &view1,
                                   const View<T> &view2) -> void {
    EXPECT_TRUE(view1.begin() == view2.begin());
    EXPECT_TRUE(view1.end() == view2.end());
  }
};

TEST(ViewTest, IteratorsTest) {
  ViewTest::iterators();
}

TEST(ViewTest, CopyAndMoveSemanticsTest) {
  ViewTest::semantics();
}
