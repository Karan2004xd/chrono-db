#pragma once

#include "constant.hpp"
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>

class Data;

class Storage {
  TEST_FRIEND(StorageTest);

public:
  using ListOfData = std::vector<Data>;
  using Container = std::map<int64_t, ListOfData>;

  using DataView = std::reference_wrapper<const Data>;
  using DataViewList = std::vector<DataView>;

  using TagsContainerData = std::unordered_map<std::string, std::pair<int64_t, DataView>>;
  using TagsContainer = std::unordered_map<int64_t, TagsContainerData>;

  using TagNameView = std::reference_wrapper<const std::string>;
  using FreeListContainer = std::unordered_map<int64_t, std::unordered_set<int64_t>>;

  Storage() = default;

  Storage(const Storage &other);
  Storage(Storage &&other) noexcept;

  auto operator=(const Storage &other) noexcept -> Storage &;
  auto operator=(Storage &&other) noexcept -> Storage &;

  auto store(int64_t timestamp,
             ListOfData &&data) noexcept -> void;

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
              std::vector<Data> &&data) noexcept -> void;

  auto erase(int64_t timestamp) noexcept -> void;
  auto erase(int64_t timestamp, std::string_view tag) noexcept -> void;

private:
  Container data_rows_;
  TagsContainer tags_map_;
  FreeListContainer free_list_;

  auto update_tag_container_data_(TagsContainerData &tag_cont_data_ref,
                                  int64_t index,
                                  DataView &&view) noexcept -> void;

  auto update_free_space_(int64_t timestamp,
                          ListOfData &&data,
                          TagsContainerData &tag_cont_data_ref)
    noexcept -> ListOfData::iterator;

  auto store_base_(int64_t timestamp,
                   ListOfData &&data) noexcept -> void;

  auto get_data_base_(int64_t timestamp) const noexcept -> DataViewList;
  auto contains_base_(int64_t timestamp,
                      std::string_view tag = {}) const noexcept -> bool;

  auto erase_base_(int64_t timestamp,
                   std::string_view tag = {}) noexcept -> void;
};
