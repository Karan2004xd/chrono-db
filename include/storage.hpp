#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

class Data;

class Storage {
public:
  using ListOfData = std::unordered_map<std::string, Data>;
  Storage() = default;

  // just testing things out, not final!!
  auto store(int64_t timestamp, std::vector<Data> data) noexcept -> void;
  auto get_data(int64_t timestamp) const noexcept 
    -> std::optional<std::reference_wrapper<const ListOfData>>;

private:
  std::map<int64_t, ListOfData> data_rows_;
};
