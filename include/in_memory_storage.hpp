#pragma once

#include "storage.hpp"
#include <map>
#include <unordered_set>
#include <unordered_map>

class Data;

class InMemoryStorage : public Storage {
  TEST_FRIEND(InMemoryStorageTest);

public:
  using ListOfData = std::vector<Data>;
  using Container = std::map<int64_t, ListOfData>;

  using TagsContainerData = std::unordered_map<std::string, std::pair<int64_t, DataView>>;
  using TagsContainer = std::unordered_map<int64_t, TagsContainerData>;

  using FreeListContainer = std::unordered_map<int64_t, std::unordered_set<int64_t>>;

  InMemoryStorage() = default;

  InMemoryStorage(const InMemoryStorage &other);
  InMemoryStorage(InMemoryStorage &&other) noexcept;

  auto operator=(const InMemoryStorage &other) noexcept -> InMemoryStorage &;
  auto operator=(InMemoryStorage &&other) noexcept -> InMemoryStorage &;

  auto store(int64_t timestamp,
             ListOfData &&data) noexcept -> void override;

  auto get_data(int64_t timestamp) const noexcept
  -> DataViewList override;
  auto get_data(int64_t timestamp, std::string_view tag_name) const noexcept
  -> std::optional<DataView> override;

  auto get_data_in_range(int64_t start_ts,
                         int64_t end_ts,
                         bool ascending = true) const noexcept
  -> std::vector<DataViewList> override;

  auto get_data_view(int64_t start_ts,
                     int64_t end_ts,
                     bool ascending = true) const noexcept
  -> View<RangeIterator<RangeDataIter>> override;

  auto contains(int64_t timestamp) const noexcept -> bool override;
  auto contains(int64_t timestamp,
                std::string_view tag_name) const noexcept -> bool override;

  auto get_tags(int64_t timestamp) const noexcept -> std::vector<TagNameView> override;

  auto get_length() const noexcept -> int64_t override;
  auto get_length(int64_t timestamp) const noexcept -> int64_t override;

  auto update(int64_t timestamp,
              std::vector<Data> &&data) noexcept -> void override;

  auto erase(int64_t timestamp) noexcept -> void override;
  auto erase(int64_t timestamp,
             std::string_view tag) noexcept -> void override;

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
