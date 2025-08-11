#include "include/data.hpp"
#include "include/storage.hpp"
#include <iostream>

int main (int argc, char *argv[]) {
  Storage storage;
  storage.store(100000032, {
    Data {"test", "hello"}, Data {"test2", 1002.223},
  });

  if (const auto &list = storage.get_data(1000000322)) {
    for (const auto &[tag, data] : list->get()) {
      std::cout << tag << " ";

      if (auto val = data.get_string_value()) {
        std::cout << *val << std::endl;

      } else if (auto val = data.get_decimal_value()) {
        std::cout << *val << std::endl;

      } else if (auto val = data.get_integer_value()) {
        std::cout << *val << std::endl;

      } else {
        std::cout << "No value found" << std::endl;
      }
    }

  } else {
    std::cout << "No data found" << std::endl;
  }
  return 0;
}
