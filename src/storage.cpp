#include "../include/storage.hpp"
#include "../include/data.hpp"

auto Storage::store(int64_t timestamp, std::vector<Data> data) noexcept -> void {
  auto &row = data_rows_[timestamp];
  auto map_of_data = ListOfData {};

  for (auto &data_obj : data) {
    map_of_data[data_obj.get_tag()] = std::move(data_obj);
  }

  row = std::move(map_of_data);
}

auto Storage::get_data(int64_t timestamp) const noexcept 
  -> std::optional<std::reference_wrapper<const ListOfData>> {

  if (!data_rows_.contains(timestamp)) return std::nullopt;
  return std::cref(data_rows_.at(timestamp));
}
