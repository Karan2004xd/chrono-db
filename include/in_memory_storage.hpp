#pragma once

#include <map>
#include <string>
#include <variant>

class InMemoryStorage {
public:
  using DataVariants = std::variant<int, float, double, std::string, long long>;
  using DataContainer = std::map<std::string, DataVariants>;

private:
  DataContainer container_;
};
