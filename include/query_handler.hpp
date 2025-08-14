#pragma once

#include "constant.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <iterator>

class Storage;
class Data;

class QueryHandler {
  TEST_FRIEND(QueryHandlerTest);

public:
  using ListOfData = std::vector<Data>;
  using DataRows = std::initializer_list<std::pair<int64_t, ListOfData>>;
  
  template <typename T>
  using ListOfValues = std::initializer_list<T>;

  QueryHandler(Storage &&db);

  QueryHandler(const QueryHandler &other) = delete;
  QueryHandler(QueryHandler &&other) noexcept;

  auto operator=(const QueryHandler &other) = delete;
  auto operator=(QueryHandler &&other) noexcept;

  auto insert(int64_t timestamp,
              ListOfData &&data,
              bool update_if_exist = false) noexcept -> void;
  auto insert(DataRows &&rows,
              bool update_if_exist = false) noexcept -> void;

  auto update(int64_t timestamp, ListOfData &&data) noexcept -> void;
  auto update(DataRows &&rows) noexcept -> void;

  auto erase(int64_t timestamp) noexcept -> void;
  auto erase(int64_t timestamp, std::string_view tag_name) noexcept -> void;

  auto erase(const ListOfValues<int64_t> &timestamps) noexcept -> void;
  auto erase(int64_t timestamp,
             const ListOfValues<std::string> &tag_names) noexcept -> void;

  template <typename Iterator, typename Func>
  auto filter(Iterator begin, Iterator end, Func &&func) -> void {
    using traits = std::iterator_traits<Iterator>;

    static_assert(!std::is_same_v<typename traits::value_type, void>,
    "Iterator type is not a valid iterator (no value types)");

    static_assert(std::is_same_v<Iterator, decltype(end)>,
    "begin and end iterators must have same types");

    using func_value_type = typename std::iterator_traits<Iterator>::value_type;
    static_assert(is_invocable_with_one_arg_<Func, func_value_type>::value,
    "Func must take exactly one argument of the iterator's value type");
  }

private:
  std::unique_ptr<Storage> db_ = nullptr;

  template <typename Func, typename Arg>
  struct is_invocable_with_one_arg_ {
    static constexpr bool value =
      std::is_invocable_v<Func, Arg> &&
      !std::is_invocable_v<Func, Arg, Arg>;
  };
};
