#pragma once

#include "iterator_wrapper.hpp"
#include <optional>
#include "data.hpp"

class Storage {
public:
  using ListOfData = std::vector<Data>;

  using DataView = std::reference_wrapper<const Data>;
  using DataViewList = std::vector<DataView>;

  using TagNameView = std::reference_wrapper<const std::string>;

  Storage() = default;

  Storage(const Storage &other) = default;
  Storage(Storage &&other) noexcept = default;

  auto operator=(const Storage &other) noexcept -> Storage & = default;
  auto operator=(Storage &&other) noexcept -> Storage & = default;

  virtual auto store(int64_t timestamp,
                     ListOfData &&data) noexcept -> void = 0;

  virtual auto get_data(int64_t timestamp) const noexcept
    -> DataViewList = 0;
  virtual auto get_data(int64_t timestamp, std::string_view tag_name) const noexcept
    -> std::optional<DataView> = 0;

  virtual auto get_data_in_range(int64_t start_ts,
                                 int64_t end_ts,
                                 bool ascending = true) const noexcept
    -> std::vector<DataViewList> = 0;

  virtual auto begin_range(int64_t start_ts,
                           int64_t end_ts,
                           bool ascending) const noexcept -> IteratorWrapper = 0;

  virtual auto end_range(int64_t start_ts,
                         int64_t end_ts,
                         bool ascending) const noexcept -> IteratorWrapper = 0;


  virtual auto contains(int64_t timestamp) const noexcept -> bool = 0;
  virtual auto contains(int64_t timestamp,
                        std::string_view tag_name) const noexcept -> bool = 0;

  virtual auto get_tags(int64_t timestamp) const noexcept
    -> std::vector<TagNameView> = 0;

  virtual auto get_length() const noexcept -> int64_t = 0;
  virtual auto get_length(int64_t timestamp) const noexcept -> int64_t = 0;

  virtual auto update(int64_t timestamp,
                      std::vector<Data> &&data) noexcept -> void = 0;

  virtual auto erase(int64_t timestamp) noexcept -> void = 0;
  virtual auto erase(int64_t timestamp,
                     std::string_view tag) noexcept -> void = 0;
};
