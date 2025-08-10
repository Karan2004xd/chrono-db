#include "../include/data.hpp"

Data::Data(const Data &other) : tag_(other.tag_),type_(other.type_) {
  data_str_ = other.data_str_ ? std::make_unique<std::string>(*other.data_str_) : nullptr;
  data_double_ = other.data_double_ ? std::make_unique<double>(*other.data_double_) : nullptr;
  data_int_ = other.data_int_ ? std::make_unique<int64_t>(*other.data_int_) : nullptr;
}

Data::Data(Data &&other) noexcept : tag_(std::move(other.tag_)), type_(std::move(other.type_)) {
  data_str_ = std::move(other.data_str_);
  data_double_ = std::move(other.data_double_);
  data_int_ = std::move(other.data_int_);
}

auto Data::operator=(const Data &other) -> Data & {
  data_str_ = other.data_str_ ? std::make_unique<std::string>(*other.data_str_) : nullptr;
  data_double_ = other.data_double_ ? std::make_unique<double>(*other.data_double_) : nullptr;
  data_int_ = other.data_int_ ? std::make_unique<int64_t>(*other.data_int_) : nullptr;

  tag_ = other.tag_;
  type_ = other.type_;
  return *this;
}

auto Data::operator=(Data &&other) noexcept -> Data & {
  data_str_ = std::move(other.data_str_);
  data_double_ = std::move(other.data_double_);
  data_int_ = std::move(other.data_int_);

  tag_ = std::move(other.tag_);
  type_ = std::move(other.type_);
  return *this;
}

auto Data::get_string_value() const noexcept -> std::optional<std::string> {
  return get_value_(DataType::STRING, data_str_);
}

auto Data::get_decimal_value() const noexcept -> std::optional<double> {
  return get_value_(DataType::DECIMAL, data_double_);
}

auto Data::get_integer_value() const noexcept -> std::optional<int64_t> {
  return get_value_(DataType::INTEGER, data_int_);
}

auto Data::get_data_type() const noexcept -> int {
  switch (type_) {
    case DataType::STRING: return 3;
    case DataType::INTEGER: return 1;
    case DataType::DECIMAL: return 2; 
  }
  return -1;
}

auto Data::get_tag() const noexcept -> const std::string & {
  return tag_;
}

auto Data::set_tag(std::string_view tag) noexcept -> void {
  tag_ = tag;
}

auto Data::reset() noexcept -> void {
  reset_base_();
}

auto Data::reset_base_() noexcept -> void {
  data_str_.reset();
  data_double_.reset();
  data_int_.reset();
}
