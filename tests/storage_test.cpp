#include "gtest/gtest.h"
#include "../include/data.hpp"
#include "../include/storage.hpp"

class StorageTest : public ::testing::Test {
public:
  static auto store() -> void {
    auto obj = Storage{};
    auto n = 4;
    auto timestamps = std::vector<int64_t> {
      1004, 1003, 1002, 1001
    };

    auto dummy_data = get_dummy_data_(n, timestamps);
    for (int i = 0; i < n; i++) {
      obj.store(timestamps[i], std::move(dummy_data[i]));
    }

    for (const auto &[ts, data] : obj.data_rows_) {
      EXPECT_EQ(ts, timestamps[--n]);
    }

    ASSERT_DEATH(obj.store(-1, {}), "");
  }

  static auto get_data() -> void {
    auto obj = Storage{};
    auto timestamps = std::vector<int64_t> {1001, 1002, 1003};
    auto n = 3;

    auto dummy_data = get_dummy_data_(n, timestamps);
    for (int i = 0; i < n; i++) {
      obj.store(timestamps[i], std::move(dummy_data[i]));
    }

    auto test_dummy_data = get_dummy_data_(n, timestamps);

    for (int i = 0; i < n; i++) {
      auto &original_data = test_dummy_data[i];
      auto returned_data_refs = obj.get_data(timestamps[i]);

      EXPECT_EQ(returned_data_refs.size(), original_data.size());

      for (const auto &data_ref : returned_data_refs) {
        auto found = false;

        for (auto &original_obj : original_data) {
          if (original_obj == data_ref.get()) {
            found = true;
            break;
          }
        }
        EXPECT_TRUE(found);
      }
    }
  }

  static auto get_data_in_range() -> void {
    auto obj = Storage{};
    auto timestamps = std::vector<int64_t> {1001, 1002, 1003};
    auto n = 3;

    auto dummy_data = get_dummy_data_(n, timestamps);
    for (int i = 0; i < n; i++) {
      obj.store(timestamps[i], std::move(dummy_data[i]));
    }

    auto test_dummy_data = get_dummy_data_(n, timestamps);

    auto returned_data_refs = obj.get_data_in_range(1001, 1003);
    EXPECT_TRUE(returned_data_refs.size() == 3);

    // Check the value of the first element
    EXPECT_EQ(returned_data_refs.front().front().get().get_timestamp(), 1001);
    EXPECT_EQ(returned_data_refs.back().front().get().get_timestamp(), 1003);

    // start timestamp > end timestamp
    returned_data_refs = obj.get_data_in_range(1005, 1003);
    EXPECT_TRUE(returned_data_refs.empty());

    // start timestamp and end timestamp both smaller then minimum 
    returned_data_refs = obj.get_data_in_range(10, 1);
    EXPECT_TRUE(returned_data_refs.empty());

    // start timestamp and end timestamp both larger then maximum 
    returned_data_refs = obj.get_data_in_range(10000, 20000);
    EXPECT_TRUE(returned_data_refs.empty());
  }

private:
  static auto get_dummy_data_(int n, const std::vector<int64_t> &timestamps)
    -> std::vector<std::vector<Data>> {

    auto data = std::vector<std::vector<Data>>(n);

    for (auto i = 0; i < n; i++) {
      auto tag = "test" + std::to_string(i);
      auto str_val = "value" + std::to_string(i);
      auto int_val = 111 * i;
      auto double_val = 17.9 * i;

      auto single_data = std::vector<Data>{
        Data {timestamps[i], tag, str_val},
        Data {timestamps[i], tag, int_val},
        Data {timestamps[i], tag, double_val}
      };

    data[i] = std::move(single_data);
    }
    return data;
  }
};

TEST_F(StorageTest, StoreDataTest) {
  StorageTest::store();
}

TEST_F(StorageTest, GetDataByTsTest) {
  StorageTest::get_data();
}

TEST_F(StorageTest, GetDataInRangeTest) {
  StorageTest::get_data_in_range();
}
