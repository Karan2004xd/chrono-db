#include "gtest/gtest.h"
#include "../include/data.hpp"
#include "../include/storage.hpp"

class StorageTest : public ::testing::Test {
public:
  static auto copy_sementics() -> void {
    auto obj = Storage{};
    obj.store(1002, { Data {"test", "hello"} });

    // Copy Constructor
    auto obj2 = Storage {obj};
    check_if_objects_equal_(obj, obj2);

    // Copy Assignment
    auto obj3 = obj2;
    check_if_objects_equal_(obj3, obj2);
  }

  static auto move_sementics() -> void {
    auto obj = Storage{};
    obj.store(1002, { Data {"test", "hello"} });
    auto temp = obj;

    // Move Constructor
    auto obj2 = Storage {std::move(obj)};
    check_if_objects_equal_(obj2, temp);
    check_if_objects_equal_(obj, Storage {});

    // Move Assignment
    auto obj3 = std::move(obj2);
    check_if_objects_equal_(obj3, temp);
    check_if_objects_equal_(obj2, Storage {});
  }

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

  static auto get_single_data() -> void {
    auto obj = Storage{};
    auto data = Data {"test", "hello"};
    obj.store(1002, {data});

    auto value = obj.get_data(1002, "test");
    EXPECT_TRUE(obj.get_data(1002, "test") != std::nullopt);
    /* EXPECT_TRUE((Data {1002, "test", "hello"}) == value.get()); */
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

  static auto check_if_objects_equal_(const Storage &obj1,
                                      const Storage &obj2) -> void {

    // Check for Data object container equality
    EXPECT_EQ(obj1.data_rows_.size(), obj2.data_rows_.size());

    auto data_row_it_1 = obj1.data_rows_.begin();
    auto data_row_it_2 = obj2.data_rows_.begin();

    while (data_row_it_1 != obj1.data_rows_.end()) {
      EXPECT_EQ(data_row_it_1->first, data_row_it_2->first);

      auto data_list_1 = data_row_it_1->second;
      auto data_list_2 = data_row_it_2->second;

      EXPECT_EQ(data_list_1.size(), data_list_2.size());
      for (size_t i = 0; i < data_list_1.size(); i++) {
        EXPECT_TRUE(data_list_1[i] == data_list_2[i]);
      }
      data_row_it_1++;
      data_row_it_2++;
    }

    // Check for Tag Container
    EXPECT_EQ(obj1.tags_map_.size(), obj2.tags_map_.size());

    auto tags_map_it_1 = obj1.tags_map_.begin();
    auto tags_map_it_2 = obj2.tags_map_.begin();

    while (tags_map_it_1 != obj1.tags_map_.end()) {
      EXPECT_EQ(tags_map_it_1->first, tags_map_it_2->first);

      const auto &tags_set_1 = tags_map_it_1->second;
      const auto &tags_set_2 = tags_map_it_2->second;

      EXPECT_EQ(tags_set_1.size(), tags_set_2.size());
      for (const auto &[tag, _] : tags_set_1) {
        EXPECT_TRUE(tags_set_2.contains(tag));
      }

      tags_map_it_1++;
      tags_map_it_2++;
    }
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

TEST_F(StorageTest, CopySementicsTest) {
  StorageTest::copy_sementics();
}

TEST_F(StorageTest, MoveSementicsTest) {
  StorageTest::move_sementics();
}

TEST_F(StorageTest, StoreDataTest) {
  StorageTest::store();
}

TEST_F(StorageTest, GetSingleDataTest) {
  StorageTest::get_single_data();
}

TEST_F(StorageTest, GetDataByTsTest) {
  StorageTest::get_data();
}

TEST_F(StorageTest, GetDataInRangeTest) {
  StorageTest::get_data_in_range();
}
