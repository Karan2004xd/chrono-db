#include "../include/storage.hpp"
#include "../include/data.hpp"
#include <algorithm>

auto Storage::store(int64_t timestamp, std::vector<Data> data) noexcept -> void {
  if (!data_rows_.empty() && timestamp > data_rows_.back().first) {
    data_rows_.emplace_back(timestamp, ListOfData {});
    index_map_[timestamp] = data_rows_.size() - 1;

  } else {
    auto it = std::lower_bound(data_rows_.begin(), data_rows_.end(), timestamp,
                               [](const auto &pair, int64_t ts) { return pair.first < ts; });

    if (it != data_rows_.end() && it->first == timestamp) {
      it->second = ListOfData {};
      index_map_[timestamp] = it - data_rows_.begin();

    } else {
      data_rows_.insert(it, {timestamp, ListOfData {}});
    }
  }

  auto &row = data_rows_[index_map_[timestamp]].second;
  for (auto &data_obj : data) {
    row[data_obj.get_tag()] = std::move(data_obj);
  }
}

auto Storage::get_data(int64_t timestamp) const noexcept 
  -> std::optional<std::reference_wrapper<const ListOfData>> {

  if (!index_map_.contains(timestamp)) return std::nullopt;
  return std::cref(data_rows_[index_map_.at(timestamp)].second);
}
