#include <gtest/gtest.h>
#include "../include/data.hpp"

struct DataTest : public ::testing::Test {
  static auto ConstructorDataAssign() -> void {
    auto data = Data {"test", "hello"};
    auto data1 = Data {"test", 10000};
    auto data2 = Data {"test", 1002.9333};
    auto data3 = Data {"test", std::string("hello world")};

    EXPECT_EQ("hello", *data.get_string_value());
  }
};

TEST_F(DataTest, ConstructorDataAssignTest) {
  DataTest::ConstructorDataAssign();
}
