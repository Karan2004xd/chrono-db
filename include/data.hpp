#pragma once

#include "constant.hpp"
#include <cassert>
#include <optional>
#include <string>

class Data {
  TEST_FRIEND(DataTest)

public:
  enum class DataType {
    INTEGER, // 1
    DECIMAL, // 2
    STRING // 3
  };

  Data() = default;

  template <typename T>
  Data(std::string_view tag, T &&value) : tag_(tag) {
    assign_value_(std::forward<T>(value));
  }

  template <typename T>
  auto set_value(T &&value) noexcept -> void {
    assign_value_(std::forward<T>(value));
  }

  Data(const Data &other);
  Data(Data &&other) noexcept;

  auto operator=(const Data &other) -> Data &;
  auto operator=(Data &&other) noexcept -> Data &;

  auto get_string_value() const noexcept -> std::optional<std::string>;
  auto get_decimal_value() const noexcept -> std::optional<double>;
  auto get_integer_value() const noexcept -> std::optional<int64_t>;
  auto get_data_type() const noexcept -> int;
  auto get_tag() const noexcept -> const std::string &;

  auto set_tag(std::string_view tag) noexcept -> void;
  auto reset(bool reset_tag) noexcept -> void;

private:
  std::unique_ptr<std::string> data_str_ = nullptr;
  std::unique_ptr<double> data_double_ = nullptr;
  std::unique_ptr<int64_t> data_int_ = nullptr;

  std::string tag_;
  DataType type_ {};


  template <typename T>
  auto assign_value_(T &&value) noexcept -> void {
    using decayed_t = std::decay_t<T>;

    static_assert(
      std::is_same_v<decayed_t, int>         ||
      std::is_same_v<decayed_t, long>        ||
      std::is_same_v<decayed_t, long long>   ||
      std::is_same_v<decayed_t, float>       ||
      std::is_same_v<decayed_t, double>      ||
      std::is_convertible_v<decayed_t, std::string>,
      "Data can only be numeric and string type"
    );

    reset_base_();

    if constexpr (std::is_same_v<decayed_t, int>     ||
                  std::is_same_v<decayed_t, long>    ||
                  std::is_same_v<decayed_t, long long>) {
      data_int_ = std::make_unique<int64_t>(static_cast<int64_t>(value));
      type_ = DataType::INTEGER;

    } else if constexpr (std::is_same_v<decayed_t, double> ||
                         std::is_same_v<decayed_t, float>) {
      assert(!std::isnan(value));

      data_double_ = std::make_unique<double>(static_cast<double>(value));
      type_ = DataType::DECIMAL;

    } else if constexpr (std::is_convertible_v<decayed_t, std::string>) {
      data_str_ = std::make_unique<std::string>(std::forward<T>(value));
      type_ = DataType::STRING;
    }
  }

  template <typename T>
  auto get_value_(const DataType &type,
                  const std::unique_ptr<T> &data) const noexcept -> std::optional<T> {
    if (type_ == type && data) return *data;
    return std::nullopt;
  }

  auto reset_base_() noexcept -> void;
};
