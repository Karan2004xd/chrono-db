#pragma once

#include "constant.hpp"
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

private:
  std::unique_ptr<Storage> db_ = nullptr;
};
