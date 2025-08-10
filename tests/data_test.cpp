#include "gtest/gtest.h"
#include "../include/data.hpp"

struct DataTest : public ::testing::Test {
  static auto constrcutor_data_assign() -> void {
    auto data = Data {"test", "hello"};
    auto data1 = Data {"test1", 10000};
    auto data2 = Data {"test2", 1002.9333};
    auto data3 = Data {"test3", std::string("hello world")};

    // Values Tests
    EXPECT_EQ("hello", *data.data_str_);
    EXPECT_EQ(10000, *data1.data_int_);
    EXPECT_EQ(1002.9333, *data2.data_double_);
    EXPECT_EQ("hello world", *data3.data_str_);

    // Tags Test
    EXPECT_EQ("test", data.tag_);
    EXPECT_EQ("test1", data1.tag_);
    EXPECT_EQ("test2", data2.tag_);
    EXPECT_EQ("test3", data3.tag_);

    // Type Test
    EXPECT_EQ(Data::DataType::STRING, data.type_);
    EXPECT_EQ(Data::DataType::INTEGER, data1.type_);
    EXPECT_EQ(Data::DataType::DECIMAL, data2.type_);
    EXPECT_EQ(Data::DataType::STRING, data3.type_);
  }

  static auto copy_semantics() -> void {
    auto data = Data {"test", "hello"};
    auto data2 = Data(data);

    check_if_data_equal_(data, data2);

    data2.tag_ = "test2";
    data = data2;

    check_if_data_equal_(data, data2);
  }

  static auto move_semantics() -> void {
    auto data = Data {"test", "hello"};
    auto data2 = Data(std::move(data));

    check_if_data_equal_(data, Data {});

    auto data3 = data2;
    data = std::move(data2);

    check_if_data_equal_(data2, Data {});
    check_if_data_equal_(data, data3);
  }

  static auto getters() -> void {
    auto data1 = Data {"test1", "hello"};
    auto data2 = Data {"test2", 100000};
    auto data3 = Data {"test3", 102.932239};

    // value getters test
    EXPECT_TRUE(check_optional_value_(data1.get_string_value(), std::string("hello")));
    EXPECT_FALSE(check_optional_value_(data1.get_decimal_value()));
    EXPECT_FALSE(check_optional_value_(data1.get_integer_value()));

    EXPECT_TRUE(check_optional_value_(data2.get_integer_value(), {100000}));
    EXPECT_FALSE(check_optional_value_(data2.get_string_value()));
    EXPECT_FALSE(check_optional_value_(data2.get_decimal_value()));

    EXPECT_TRUE(check_optional_value_(data3.get_decimal_value(), {102.932239}));
    EXPECT_FALSE(check_optional_value_(data3.get_string_value()));
    EXPECT_FALSE(check_optional_value_(data3.get_integer_value()));

    // Data Types
    EXPECT_EQ(data2.get_data_type(), 1);
    EXPECT_EQ(data3.get_data_type(), 2);
    EXPECT_EQ(data1.get_data_type(), 3);

    // Tag
    EXPECT_EQ(data1.get_tag(), "test1");
  }

  static auto setters() -> void {
    auto data = Data {"test", "hello"};

    // value setter
    data.set_value(10000);
    check_if_data_equal_(data, Data {"test", 10000});

    data.set_value(100.29293);
    check_if_data_equal_(data, Data {"test", 100.29293});

    // tag setter
    data.set_tag("test2");
    EXPECT_EQ(data.tag_, "test2");
  }

  static auto reset_data() -> void {
    auto data = Data {"test", "hello"};

    // with tag reset
    data.reset(true);
    check_if_data_equal_(data, Data {});

    // without tag reset
    data = Data {"test", "hello"};
    data.reset(false);
    EXPECT_EQ(data.tag_, "test");
  }

  static auto extreme_values() -> void {
    // Integer values
    auto max_int_value = std::numeric_limits<int64_t>::max();
    auto int_value = Data {"int_value", max_int_value};

    EXPECT_TRUE(check_optional_value_(int_value.get_integer_value(), max_int_value));

    auto min_int_value = std::numeric_limits<int64_t>::min();
    int_value.set_value(min_int_value);

    EXPECT_TRUE(check_optional_value_(int_value.get_integer_value(), min_int_value));

    // Decimal Values
    auto max_decimal_value = std::numeric_limits<double>::max();
    auto decimal_value = Data {"decimal_value", max_decimal_value};

    EXPECT_TRUE(check_optional_value_(decimal_value.get_decimal_value(),
                                      max_decimal_value));

    auto inf_decimal_value = std::numeric_limits<double>::infinity();
    decimal_value.set_value(inf_decimal_value);

    EXPECT_TRUE(check_optional_value_(decimal_value.get_decimal_value(),
                                      inf_decimal_value));

    auto nan_decimal_value = std::numeric_limits<double>::quiet_NaN();
    EXPECT_DEATH(decimal_value.set_value(nan_decimal_value), "");

    // String Values
    auto str_value = Data {"str_value", ""};
    EXPECT_TRUE(check_optional_value_(str_value.get_string_value(), {}));

    auto large_str = std::string(10'000'000, 'x');
    str_value.set_value(large_str);

    EXPECT_TRUE(check_optional_value_(str_value.get_string_value(), large_str));
  }

private:
  static auto check_if_data_equal_(const Data &d1, const Data &d2) -> void {
    EXPECT_EQ(d1.type_, d2.type_);
    EXPECT_EQ(d1.tag_, d2.tag_);

    EXPECT_TRUE((!d1.data_str_ && !d2.data_str_) 
                || *d1.data_str_ == *d2.data_str_);

    EXPECT_TRUE((!d1.data_int_ && !d2.data_int_) 
                || *d1.data_int_ == *d2.data_int_);

    EXPECT_TRUE((!d1.data_double_ && !d2.data_double_) 
                || *d1.data_double_ == *d2.data_double_);
  }

  template <typename T>
  static auto check_optional_value_(const std::optional<T> &optional_value,
                                    const T &value = {}) -> bool {
    return optional_value ? *optional_value == value : false;
  }
};

TEST_F(DataTest, ConstructorDataAssignTest) {
  DataTest::constrcutor_data_assign();
}

TEST_F(DataTest, CopySemanticsTest) {
  DataTest::copy_semantics();
}

TEST_F(DataTest, MoveSemanticsTest) {
  DataTest::move_semantics();
}

TEST_F(DataTest, GettersTest) {
  DataTest::getters();
}

TEST_F(DataTest, SettersTest) {
  DataTest::setters();
}

TEST_F(DataTest, ResetDataTest) {
  DataTest::reset_data();
}

TEST_F(DataTest, ExtremeValuesTest) {
  DataTest::extreme_values();
}
