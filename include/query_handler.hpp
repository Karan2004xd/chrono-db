#pragma once

#include "view.hpp"
#include <functional>

class Storage;
class Data;

class QueryHandler {
  TEST_FRIEND(QueryHandlerTest);

public:
  using ListOfData = std::vector<Data>;
  using DataRows = std::initializer_list<std::pair<int64_t, ListOfData>>;
  
  template <typename T>
  using ListOfValues = std::initializer_list<T>;

  template <typename T>
  using DataView = std::reference_wrapper<const T>;
  using DataViewList = std::vector<DataView<Data>>;

  using Row = std::vector<std::pair<int64_t, DataViewList>>;

  using PredicateForRow = std::function<bool(int64_t ts)>;
  using PredicateForColumn = std::function<bool(const Data &)>;

  using IteratorView = View<RangeIterator<RangeDataIter>>;

  enum class RowOrder {
    ASC, DSC
  };

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

  auto get_data(int64_t start_ts,
                int64_t end_ts,
                PredicateForRow &&func,
                const RowOrder &order,
                int limit = -1) const noexcept -> Row;

  auto get_data(int64_t start_ts,
                int64_t end_ts,
                PredicateForColumn &&func,
                const RowOrder &order,
                int limit = -1) const noexcept -> Row;

  auto get_data(int64_t start_ts,
                int64_t end_ts,
                const RowOrder &order,
                int limit = -1) const noexcept -> Row;

private:
  std::unique_ptr<Storage> db_ = nullptr;

  auto get_data_helper_(int64_t start_ts,
                        int64_t end_ts,
                        const RowOrder &order,
                        int limit) const noexcept
  -> std::tuple<bool, IteratorView, bool>;
};
