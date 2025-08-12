#pragma once

#include "constant.hpp"
#include <map>
#include <optional>
#include <unordered_map>

class Data;

class Storage {
  TEST_FRIEND(StorageTest)

public:
  using ListOfData = std::vector<Data>;
  using Container = std::map<int64_t, ListOfData>;

  using ConstDataRef = std::reference_wrapper<const Data>;
  using ConstDataRefList = std::vector<ConstDataRef>;

  using TagsContainer = std::unordered_map<int64_t, std::unordered_map<std::string, ConstDataRef>>;
  using ConstTagNameRef = std::reference_wrapper<const std::string>;

  Storage() = default;

  Storage(const Storage &other);
  Storage(Storage &&other) noexcept;

  auto operator=(const Storage &other) noexcept -> Storage &;
  auto operator=(Storage &&other) noexcept -> Storage &;

  auto store(int64_t timestamp, std::vector<Data> &&data) noexcept -> void;

  auto get_data(int64_t timestamp) const noexcept
    -> ConstDataRefList;

  auto get_data(int64_t timestamp, std::string_view tag_name) const noexcept
    -> std::optional<ConstDataRef>;

  auto get_data_in_range(int64_t start_ts, int64_t end_ts) const noexcept
    -> std::vector<ConstDataRefList>;

  auto contains(int64_t timestamp) const noexcept -> bool;
  auto get_tags(int64_t timestamp) const noexcept -> std::vector<ConstTagNameRef>;

private:
  Container data_rows_;
  TagsContainer tags_map_;

  auto store_base_(int64_t timestamp, std::vector<Data> &&data) noexcept -> void;
  auto get_data_base_(int64_t timestamp) const noexcept -> ConstDataRefList;
};
