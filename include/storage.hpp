#pragma once

#include "constant.hpp"
#include <map>
#include <unordered_set>

class Data;

class Storage {
  TEST_FRIEND(StorageTest)

public:
  using ListOfData = std::vector<Data>;
  using Container = std::map<int64_t, ListOfData>;

  using DataView = std::reference_wrapper<const Data>;
  using DataViewList = std::vector<DataView>;

  using TagsContainer = std::unordered_map<
    int64_t, std::unordered_map<std::string, std::pair<int64_t, DataView>>>;

  using TagNameView = std::reference_wrapper<const std::string>;
  using FreeListContainer = std::unordered_map<int64_t, std::unordered_set<int64_t>>;

  using DataRows = std::initializer_list<std::pair<int64_t, ListOfData>>;

  Storage() = default;

  Storage(const Storage &other);
  Storage(Storage &&other) noexcept;

  auto operator=(const Storage &other) noexcept -> Storage &;
  auto operator=(Storage &&other) noexcept -> Storage &;

  auto store(int64_t timestamp,
             std::vector<Data> &&data,
             bool update_if_exist = false) noexcept -> void;

  auto store(DataRows &&rows, bool update_if_exist = false) noexcept -> void;

  auto get_data(int64_t timestamp) const noexcept
    -> DataViewList;
  auto get_data(int64_t timestamp, std::string_view tag_name) const noexcept
    -> std::optional<DataView>;
  auto get_data_in_range(int64_t start_ts, int64_t end_ts) const noexcept
    -> std::vector<DataViewList>;

  auto contains(int64_t timestamp) const noexcept -> bool;
  auto contains(int64_t timestamp, std::string_view tag_name) const noexcept -> bool;

  auto get_tags(int64_t timestamp) const noexcept -> std::vector<TagNameView>;

  auto get_length() const noexcept -> int64_t;
  auto get_length(int64_t timestamp) const noexcept -> int64_t;

  auto update(int64_t timestamp,
              std::vector<Data> &&data,
              bool insert_if_not_exist = false) noexcept -> void;

  auto erase(int64_t timestamp) noexcept -> void;
  auto erase(int64_t timestamp, std::string_view tag) noexcept -> void;

  auto erase(const std::initializer_list<int64_t> &timestamps) noexcept -> void;
  auto erase(int64_t timestamp,
             const std::initializer_list<std::string> &tags) noexcept -> void;

private:
  Container data_rows_;
  TagsContainer tags_map_;
  FreeListContainer free_list_;

  auto store_base_(int64_t timestamp, std::vector<Data> &&data) noexcept -> void;
  auto get_data_base_(int64_t timestamp) const noexcept -> DataViewList;
  auto contains_base_(int64_t timestamp,
                      std::string_view tag = {}) const noexcept -> bool;

  auto erase_base_(int64_t timestamp,
                   std::string_view tag = {}) noexcept -> void;
};
