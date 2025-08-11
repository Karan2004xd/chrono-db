#include "../include/storage.hpp"
#include "../include/data.hpp"

Storage::Storage(const Storage &other)
  : data_rows_(other.data_rows_), tags_map_(other.tags_map_) {}

Storage::Storage(Storage &&other) noexcept
  : data_rows_(std::move(other.data_rows_)),
    tags_map_(std::move(other.tags_map_)) {}

auto Storage::operator=(const Storage &other) noexcept -> Storage & {
  data_rows_ = other.data_rows_;
  tags_map_ = other.tags_map_;
  return *this;
}

auto Storage::operator=(Storage &&other) noexcept -> Storage & {
  data_rows_ = std::move(other.data_rows_);
  tags_map_ = std::move(other.tags_map_);
  return *this;
}

auto Storage::store(int64_t timestamp, std::vector<Data> &&data) noexcept -> void {
  store_base_(timestamp, std::forward<decltype(data)>(data));
}

auto Storage::get_data(int64_t timestamp) const noexcept -> ConstDataRefList {
  return get_data_base_(timestamp);
}

auto Storage::get_data_in_range(int64_t start_ts, int64_t end_ts) const noexcept 
  -> std::vector<ConstDataRefList> {

  auto range_data = std::vector<ConstDataRefList> {};

  if (start_ts > end_ts || data_rows_.empty()) return range_data;

  auto start_it = data_rows_.lower_bound(start_ts);
  auto end_it = data_rows_.upper_bound(end_ts);

  for (auto it = start_it; it != end_it; it++) {
    auto ts_data_list = get_data_base_(it->first);
    range_data.emplace_back(std::move(ts_data_list));
  }
  return range_data;
}

auto Storage::store_base_(int64_t timestamp,
                          std::vector<Data> &&data) noexcept -> void {

  assert(timestamp >= 0);

  auto &row = data_rows_[timestamp];
  for (auto &data_obj : data) {
    data_obj.set_timestamp(timestamp);
    tags_map_[timestamp].insert(data_obj.get_tag());
    row.emplace_back(std::move(data_obj));
  }
}

auto Storage::get_data_base_(int64_t timestamp) const noexcept
  -> ConstDataRefList {

  if (!data_rows_.contains(timestamp)) return {};
  auto data_list = ConstDataRefList{};

  const auto &data_stored = data_rows_.at(timestamp);
  for (const auto &data : data_stored) {
    data_list.push_back(std::cref(data));
  }
  return data_list;
}
