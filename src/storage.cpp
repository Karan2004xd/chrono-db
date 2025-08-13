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

auto Storage::store(int64_t timestamp,
                    std::vector<Data> &&data,
                    bool update_if_exist) noexcept -> void {
  if (contains_base_(timestamp) && !update_if_exist) return ;
  store_base_(timestamp, std::forward<decltype(data)>(data));
}

auto Storage::store(DataRows &&rows, bool update_if_exist) noexcept -> void {
  for (auto row : rows) {
    auto timestamp = row.first;

    if (contains_base_(timestamp) && !update_if_exist) break;
    store_base_(timestamp, std::forward<decltype(row.second)>(row.second));
  }
}

auto Storage::get_data(int64_t timestamp) const noexcept -> DataViewList {
  return get_data_base_(timestamp);
}

auto Storage::get_data(int64_t timestamp, std::string_view tag_name) const noexcept
  -> std::optional<DataView> {

  if (!contains_base_(timestamp, tag_name)) return std::nullopt;
  return std::cref(tags_map_.at(timestamp).at(std::string(tag_name)).second);
}

auto Storage::get_data_in_range(int64_t start_ts, int64_t end_ts) const noexcept 
  -> std::vector<DataViewList> {

  auto range_data = std::vector<DataViewList> {};

  if (start_ts > end_ts || data_rows_.empty()) return range_data;

  auto start_it = data_rows_.lower_bound(start_ts);
  auto end_it = data_rows_.upper_bound(end_ts);

  for (auto it = start_it; it != end_it; it++) {
    auto ts_data_list = get_data_base_(it->first);
    range_data.emplace_back(std::move(ts_data_list));
  }
  return range_data;
}

auto Storage::contains(int64_t timestamp) const noexcept -> bool {
  return contains_base_(timestamp);
}

auto Storage::contains(int64_t timestamp,
                       std::string_view tag_name) const noexcept -> bool {
  return contains_base_(timestamp, tag_name);
}

auto Storage::get_tags(int64_t timestamp) const noexcept
  -> std::vector<TagNameView> {

  auto tags_list = std::vector<TagNameView> {};
  if (!tags_map_.contains(timestamp)) return tags_list;

  const auto &ts_tags_map = tags_map_.at(timestamp);

  for (const auto &[tag, _] : ts_tags_map) {
    tags_list.emplace_back(std::cref(tag));
  }
  return tags_list;
}

auto Storage::get_length() const noexcept -> int64_t {
  return data_rows_.size();
}

auto Storage::get_length(int64_t timestamp) const noexcept -> int64_t {
  if (!contains_base_(timestamp)) return 0;
  return tags_map_.at(timestamp).size();
}

auto Storage::update(int64_t timestamp,
                     std::vector<Data> &&data,
                     bool insert_if_not_exist) noexcept -> void {
  if (!insert_if_not_exist && !contains_base_(timestamp)) return ;

  store_base_(timestamp, std::forward<decltype(data)>(data));
}

auto Storage::erase(int64_t timestamp) noexcept -> void {
  erase_base_(timestamp);
}

auto Storage::erase(int64_t timestamp, std::string_view tag) noexcept -> void {
  erase_base_(timestamp, tag);
}

auto Storage::erase(const std::initializer_list<int64_t> &timestamps) noexcept -> void {
  for (const auto &timestamp : timestamps) {
    erase_base_(timestamp);
  }
}

auto Storage::erase(int64_t timestamp,
                    const std::initializer_list<std::string> &tags) noexcept -> void {
  for (const auto &tag : tags) {
    erase_base_(timestamp, tag);
  }
}

auto Storage::store_base_(int64_t timestamp,
                          std::vector<Data> &&data) noexcept -> void {

  assert(timestamp >= 0);

  auto &free_idx_list = free_list_[timestamp];
  auto data_it = data.begin();

  auto &row = data_rows_[timestamp];
  auto &tag_map_row = tags_map_[timestamp];

  auto available_idx = std::unordered_set<int64_t>{};
  for (size_t i = 0; i < row.size(); i++) {
    if (data_it != data.end()) {
      data_it->set_timestamp(timestamp);
      data_it++;
    }

    available_idx.insert(i);
  }

  data_it = data.begin();

  // Occupy the free index spots
  for (auto &free_idx : free_idx_list) {
    if (data_it == data.end()) break;
    row[free_idx] = std::move(*data_it);

    available_idx.erase(free_idx);
    free_idx_list.erase(free_idx);

    data_it++;
  }

  // Occupy the index that are left out and available
  for (const auto &left_idx : available_idx) {
    if (data_it == data.end()) break;
    row[left_idx] = std::move(*data_it);
    data_it++;
  }

  // Create new index if no other spot is available
  while (data_it != data.end()) {
    data_it->set_timestamp(timestamp);
    auto idx = data_it - data.begin();

    row.emplace_back(std::move(*data_it));
    auto tag_metadata = std::make_pair<int64_t, DataView>(
      idx, std::cref(row.back())
    );

    tag_map_row.insert({row.back().get_tag(), std::move(tag_metadata)});
    data_it++;
  }
}

auto Storage::get_data_base_(int64_t timestamp) const noexcept
  -> DataViewList {

  if (!data_rows_.contains(timestamp)) return {};
  auto data_list = DataViewList{};

  const auto &data_stored = data_rows_.at(timestamp);
  for (const auto &data : data_stored) {
    data_list.push_back(std::cref(data));
  }
  return data_list;
}

auto Storage::contains_base_(int64_t timestamp,
                             std::string_view tag) const noexcept -> bool {

  assert(data_rows_.contains(timestamp) == tags_map_.contains(timestamp));

  if (!data_rows_.contains(timestamp)) return false;
  if (tag.empty()) return true;

  auto tag_str = std::string(tag);
  if (!tags_map_.at(timestamp).contains(tag_str)) return false;

  auto data_idx = tags_map_.at(timestamp).at(tag_str).first;
  return !free_list_.at(timestamp).contains(data_idx);
}

auto Storage::erase_base_(int64_t timestamp,
                          std::string_view tag) noexcept -> void {

  if (!contains_base_(timestamp, tag)) return ;

  auto tag_str = std::string(tag);
  auto &tags_map_list = tags_map_.at(timestamp);

  if (!tag.empty()) {
    auto idx = tags_map_list.at(tag_str).first;
    free_list_.at(timestamp).insert(idx);
    tags_map_list.erase(tag_str);

  } else {
    tags_map_.erase(timestamp);
    data_rows_.erase(timestamp);
    free_list_.erase(timestamp);
  }
}
