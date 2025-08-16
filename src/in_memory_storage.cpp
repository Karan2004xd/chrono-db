#include "../include/in_memory_storage.hpp"
#include "../include/view.hpp"

InMemoryStorage::InMemoryStorage(const InMemoryStorage &other)
  : data_rows_(other.data_rows_), tags_map_(other.tags_map_) {}

InMemoryStorage::InMemoryStorage(InMemoryStorage &&other) noexcept
  : data_rows_(std::move(other.data_rows_)),
  tags_map_(std::move(other.tags_map_)) {}

auto InMemoryStorage::operator=(const InMemoryStorage &other) noexcept -> InMemoryStorage & {
  data_rows_ = other.data_rows_;
  tags_map_ = other.tags_map_;
  return *this;
}

auto InMemoryStorage::operator=(InMemoryStorage &&other) noexcept -> InMemoryStorage & {
  data_rows_ = std::move(other.data_rows_);
  tags_map_ = std::move(other.tags_map_);
  return *this;
}

auto InMemoryStorage::store(int64_t timestamp,
                            std::vector<Data> &&data) noexcept -> void {
  store_base_(timestamp, std::forward<decltype(data)>(data));
}

auto InMemoryStorage::get_data(int64_t timestamp) const noexcept -> DataViewList {
  return get_data_base_(timestamp);
}

auto InMemoryStorage::get_data(int64_t timestamp, std::string_view tag_name) const noexcept
-> std::optional<DataView> {

  if (!contains_base_(timestamp, tag_name)) return std::nullopt;
  return std::cref(tags_map_.at(timestamp).at(std::string(tag_name)).second);
}

auto InMemoryStorage::get_data_in_range(int64_t start_ts,
                                        int64_t end_ts,
                                        bool ascending) const noexcept 
-> std::vector<DataViewList> {

  auto range_data = std::vector<DataViewList> {};

  if (start_ts > end_ts || data_rows_.empty()) return range_data;

  auto start_it = data_rows_.lower_bound(start_ts);
  auto end_it = data_rows_.upper_bound(end_ts);

  auto range_data_len = std::distance(start_it, end_it);
  range_data.resize(range_data_len);

  auto range_data_idx = ascending ? 0 : range_data_len - 1;

  for (auto it = start_it; it != end_it; it++) {
    auto ts_data_list = get_data_base_(it->first);
    range_data.at(range_data_idx) = std::move(ts_data_list);

    range_data_idx = ascending ? range_data_idx + 1 : range_data_idx - 1;
  }
  return range_data;
}

auto InMemoryStorage::get_data_view(int64_t start_ts,
                                    int64_t end_ts,
                                    bool ascending) const noexcept 
  -> View<RangeIterator<RangeDataIter>> {

  auto start = data_rows_.lower_bound(start_ts);
  auto end = data_rows_.upper_bound(end_ts);

  if (!ascending) {
    start = data_rows_.lower_bound(end_ts);
    end = data_rows_.lower_bound(start_ts);
  }
  auto data_view = make_data_view(start, end, ascending);
  return data_view;
}

auto InMemoryStorage::contains(int64_t timestamp) const noexcept -> bool {
  return contains_base_(timestamp);
}

auto InMemoryStorage::contains(int64_t timestamp,
                               std::string_view tag_name) const noexcept -> bool {
  return contains_base_(timestamp, tag_name);
}

auto InMemoryStorage::get_tags(int64_t timestamp) const noexcept
-> std::vector<TagNameView> {

  auto tags_list = std::vector<TagNameView> {};
  if (!tags_map_.contains(timestamp)) return tags_list;

  const auto &ts_tags_map = tags_map_.at(timestamp);

  for (const auto &[tag, _] : ts_tags_map) {
    tags_list.emplace_back(std::cref(tag));
  }
  return tags_list;
}

auto InMemoryStorage::get_length() const noexcept -> int64_t {
  return data_rows_.size();
}

auto InMemoryStorage::get_length(int64_t timestamp) const noexcept -> int64_t {
  if (!contains_base_(timestamp)) return 0;
  return tags_map_.at(timestamp).size();
}

auto InMemoryStorage::update(int64_t timestamp,
                             std::vector<Data> &&data) noexcept -> void {
  store_base_(timestamp, std::forward<decltype(data)>(data));
}

auto InMemoryStorage::erase(int64_t timestamp) noexcept -> void {
  erase_base_(timestamp);
}

auto InMemoryStorage::erase(int64_t timestamp, std::string_view tag) noexcept -> void {
  erase_base_(timestamp, tag);
}

auto InMemoryStorage::update_tag_container_data_(TagsContainerData &tag_cont_data_ref,
                                                 int64_t index,
                                                 DataView &&view) noexcept -> void {
  auto data_pair = std::make_pair(
    index, std::forward<decltype(view)>(view)
  );
  tag_cont_data_ref.insert({view.get().get_tag(), std::move(data_pair)});
}

auto InMemoryStorage::update_free_space_(int64_t timestamp,
                                         ListOfData &&data,
                                         TagsContainerData &tag_cont_data_ref) 
noexcept -> ListOfData::iterator {

  auto &free_idx_list = free_list_[timestamp];
  auto &row = data_rows_[timestamp];

  auto available_idx = std::unordered_set<int64_t>{};
  auto data_it = data.begin();

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
    update_tag_container_data_(tag_cont_data_ref,
                               free_idx,
                               std::cref(row[free_idx]));

    available_idx.erase(free_idx);
    free_idx_list.erase(free_idx);

    data_it++;
  }

  // Occupy the index that are left out and available
  for (const auto &left_idx : available_idx) {
    if (data_it == data.end()) break;

    row[left_idx] = std::move(*data_it);
    update_tag_container_data_(tag_cont_data_ref,
                               left_idx,
                               std::cref(row[left_idx]));
    data_it++;
  }
  return data_it;
}

auto InMemoryStorage::store_base_(int64_t timestamp,
                                  ListOfData &&data) noexcept -> void {

  assert(timestamp >= 0);

  auto &row = data_rows_[timestamp];
  auto &tag_map_row = tags_map_[timestamp];
  tag_map_row = {};

  auto data_it = update_free_space_(timestamp,
                                    std::forward<decltype(data)>(data),
                                    tag_map_row);

  // Create new index if no other spot is available
  while (data_it != data.end()) {
    data_it->set_timestamp(timestamp);
    auto idx = data_it - data.begin();

    row.emplace_back(std::move(*data_it));
    update_tag_container_data_(tag_map_row, idx, std::cref(row.back()));
    data_it++;
  }
}

auto InMemoryStorage::get_data_base_(int64_t timestamp) const noexcept
-> DataViewList {

  if (!data_rows_.contains(timestamp)) return {};
  auto data_list = DataViewList{};

  const auto &data_stored = data_rows_.at(timestamp);
  for (const auto &data : data_stored) {
    data_list.push_back(std::cref(data));
  }
  return data_list;
}

auto InMemoryStorage::contains_base_(int64_t timestamp,
                                     std::string_view tag) const noexcept -> bool {

  assert(data_rows_.contains(timestamp) == tags_map_.contains(timestamp));

  if (!data_rows_.contains(timestamp)) return false;
  if (tag.empty()) return true;

  auto tag_str = std::string(tag);
  if (!tags_map_.at(timestamp).contains(tag_str)) return false;

  auto data_idx = tags_map_.at(timestamp).at(tag_str).first;
  return !free_list_.at(timestamp).contains(data_idx);
}

auto InMemoryStorage::erase_base_(int64_t timestamp,
                                  std::string_view tag) noexcept -> void {

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
