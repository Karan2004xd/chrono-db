#pragma once

#include <unordered_map>
#include <vector>
#include <optional>

class Data;

class Storage {
public:
  using ListOfData = std::unordered_map<std::string, Data>;
  using Container = std::vector<std::pair<int64_t, ListOfData>>;

  Storage() = default;

  auto store(int64_t timestamp, std::vector<Data> data) noexcept -> void;
  auto get_data(int64_t timestamp) const noexcept 
    -> std::optional<std::reference_wrapper<const ListOfData>>;

private:
  Container data_rows_;
  std::unordered_map<int64_t, size_t> index_map_;
};
