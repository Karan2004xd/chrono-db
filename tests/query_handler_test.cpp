#include "gtest/gtest.h"
#include "../include/query_handler.hpp"
#include "../include/in_memory_storage.hpp"
#include "../include/storage_factory.hpp"
#include "../include/data.hpp"

class QueryHandlerTest {
public:
  static auto constructor_test() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());
    EXPECT_FALSE(obj.db_ == nullptr);
  }

  static auto semantics() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());
    auto obj2 = std::move(obj);

    EXPECT_EQ(obj.db_, nullptr);
    EXPECT_NE(obj2.db_, nullptr);
  }

  static auto single_insert() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());

    // insert without update
    obj.insert(1002, { Data {"test", "value"} });
    EXPECT_TRUE(obj.db_->contains(1002));
    EXPECT_TRUE(obj.db_->contains(1002, "test"));

    // insert with update
    obj.insert(1002, { Data {"test1", "value1"} }, true);
    EXPECT_FALSE(obj.db_->contains(1002, "test"));
    EXPECT_TRUE(obj.db_->contains(1002, "test1"));
  }

  static auto bulk_insert() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());
    auto timestamps = std::vector<int64_t>{1, 2, 3, 4};

    obj.insert({
      {timestamps[0], { Data {"test1", "value1"}} },
      {timestamps[1], { Data {"test2", "value2"}} },
      {timestamps[2], { Data {"test3", "value3"}} },
      {timestamps[3], { Data {"test4", "value4"}} },
    });

    EXPECT_EQ(obj.db_->get_length(), timestamps.size());

    for (int i = 1; i <= 4; i++) {
      EXPECT_TRUE(obj.db_->contains(i));
    }
  }

  static auto single_update() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());

    obj.insert(1, { Data {"test1", "value1"}, Data {"test1_2", "value1_2"} });
    obj.update(1, { Data {"test2", "value2"} });

    EXPECT_FALSE(obj.db_->contains(1, "test1"));
    EXPECT_TRUE(obj.db_->contains(1, "test2"));
    EXPECT_EQ(obj.db_->get_length(1), 1);

    obj.update(3, { Data {"test3", "value3"} });
    EXPECT_FALSE(obj.db_->contains(3));
  }

  static auto bulk_update() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());

    obj.insert({
      {1, { Data {"test1", "value1"}} },
      {2, { Data {"test2", "value2"}} },
      {3, { Data {"test3", "value3"}} },
      {4, { Data {"test4", "value4"}} },
    });

    obj.update({
      {1, { Data {"test0", "value0"}} },
      {2, { Data {"test2", "value2"}} },
      {3, { Data {"test6", "value6"}} },
      {4, { Data {"test4", "value4"}} },
    });

    EXPECT_EQ(obj.db_->get_length(), 4);

    EXPECT_FALSE(obj.db_->contains(1, "test1"));
    EXPECT_FALSE(obj.db_->contains(3, "test3"));

    EXPECT_TRUE(obj.db_->contains(1, "test0"));
    EXPECT_TRUE(obj.db_->contains(2, "test2"));
    EXPECT_TRUE(obj.db_->contains(3, "test6"));
    EXPECT_TRUE(obj.db_->contains(4, "test4"));
  }

  static auto single_erase() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());

    obj.insert(1, { Data {"test1", "value1"}, Data {"test2", "value2"}});
    obj.erase(1, "test1");
    EXPECT_FALSE(obj.db_->contains(1, "test1"));
    EXPECT_TRUE(obj.db_->contains(1, "test2"));
    EXPECT_EQ(obj.db_->get_length(1), 1);

    obj.erase(1);
    EXPECT_FALSE(obj.db_->contains(1));
  }

  static auto bulk_erase() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());

    obj.insert({
      {1, { Data {"test0", "value0"}, Data {"test1", "value1"}, Data {"test2", "value2"}} },
      {2, { Data {"test1", "value1"}} },
      {3, { Data {"test2", "value2"}} },
      {4, { Data {"test3", "value3"}} },
    });

    obj.erase(1, {"test0", "test1"});
    EXPECT_EQ(obj.db_->get_length(1), 1);

    obj.erase({2, 3});
    EXPECT_EQ(obj.db_->get_length(), 2);
  }

  static auto get_data() -> void {
    auto obj = QueryHandler(StorageFactory::get_in_memory_storage());
    std::vector<int> vec {1, 2, 5, 4};

    obj.filter(vec.begin(), vec.end(), [](const auto &val) {
      return val >= 2;
    });
  }
};

TEST(QueryHandlerTest, ConstructorTest) {
  QueryHandlerTest::constructor_test();
}

TEST(QueryHandlerTest, SemanticsTest) {
  QueryHandlerTest::semantics();
}

TEST(QueryHandlerTest, SingleInsertTest) {
  QueryHandlerTest::single_insert();
}

TEST(QueryHandlerTest, BulkInsertTest) {
  QueryHandlerTest::bulk_insert();
}

TEST(QueryHandlerTest, SingleUpdateTest) {
  QueryHandlerTest::single_update();
}

TEST(QueryHandlerTest, BulkUpdateTest) {
  QueryHandlerTest::bulk_update();
}

TEST(QueryHandlerTest, SingleEraseTest) {
  QueryHandlerTest::single_erase();
}

TEST(QueryHandlerTest, BulkEraseTest) {
  QueryHandlerTest::bulk_erase();
}

TEST(QueryHandlerTest, GetDataTest) {
  QueryHandlerTest::get_data();
}
