#include "../include/storage_factory.hpp"
#include "../include/in_memory_storage.hpp"

auto StorageFactory::get_in_memory_storage() noexcept -> InMemoryStorage {
  static InMemoryStorage in_memory_storage;
  return in_memory_storage;
}
