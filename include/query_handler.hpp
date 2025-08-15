#pragma once

#include "constant.hpp"
#include <functional>
#include <memory>
#include <vector>

class Storage;
class Data;

class QueryHandler {
  TEST_FRIEND(QueryHandlerTest);

public:
  using ListOfData = std::vector<Data>;
  using DataRows = std::initializer_list<std::pair<int64_t, ListOfData>>;
  
  template <typename T>
  using ListOfValues = std::initializer_list<T>;

  using DataView = std::reference_wrapper<Data>;
  using DataViewList = std::vector<DataView>;

  using Row = std::vector<std::pair<int64_t, DataViewList>>;

  template <typename T>
  QueryHandler(T &&db) {
    using decayed_T = std::decay_t<T>;
    static_assert(std::is_base_of_v<Storage, decayed_T>, "T must derive from Storage class");
    db_ = std::make_unique<decayed_T>(std::forward<T>(db));
  }

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

  // template <typename Iterator, typename Func>
  // auto filter(Iterator begin, Iterator end, Func &&func) -> void {
  //   using traits = std::iterator_traits<Iterator>;

  //   static_assert(!std::is_same_v<typename traits::value_type, void>,
  //   "Iterator type is not a valid iterator (no value types)");

  //   static_assert(std::is_same_v<Iterator, decltype(end)>,
  //   "begin and end iterators must have same types");

  //   using func_value_type = typename std::iterator_traits<Iterator>::value_type;
  //   static_assert(is_invocable_with_n_arg_<Func, func_value_type>::count <= 2,
  //   "Func must take exactly one argument of the iterator's value type");
  // }

private:
  std::unique_ptr<Storage> db_ = nullptr;
  using Predicate = std::function<bool(const DataView)>;

  template <typename Func, typename... Args>
  struct is_invocable_with_n_arg_ {
    static constexpr bool value = std::is_invocable_r_v<bool, Func, Args...>;
    static constexpr size_t count = sizeof...(Args);
  };

  auto get_data_base_(int64_t start_ts,
                      int64_t end_ts,
                      Predicate &&func,
                      int limit = -1) const -> Row;
};
