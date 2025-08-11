#include "include/data.hpp"
#include "include/storage.hpp"
#include <iostream>

auto print_data(const std::vector<Storage::ConstDataRef> &list) -> void {
  if (list.empty()) return ;

  std::cout << list.front().get().get_timestamp() << std::endl;
  for (const auto &data : list) {
    auto obj = data.get();
    std::cout << obj.get_tag() << " ";

    if (auto val = obj.get_string_value()) {
      std::cout << *val << std::endl;

    } else if (auto val = obj.get_decimal_value()) {
      std::cout << *val << std::endl;

    } else if (auto val = obj.get_integer_value()) {
      std::cout << *val << std::endl;

    } else {
      std::cout << "No value found" << std::endl;
    }
  }
}

int main (int argc, char *argv[]) {
  Storage storage;
  storage.store(100000032, {
    Data {"test", "hello"}, Data {"test2", 1002.223},
  });

  storage.store(100000031, {
    Data {"test", "hello"}, Data {"test2", "hello world"},
  });

  storage.store(100000030, {
    Data {"test", "hello"}, Data {"test2", 1002.223},
  });

  storage.store(100000029, {
    Data {"test", "hello"}, Data {"test2", "hello world"},
  });

  /* print_data(storage.get_data(100000032)); */

  const auto &range_data = storage.get_data_in_range(100000020, 100000050);

  for (const auto &obj : range_data) {
    print_data(obj);
    std::cout << "\n";
  }
  return 0;
}
