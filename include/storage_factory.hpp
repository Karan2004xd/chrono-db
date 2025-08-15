#pragma once

class InMemoryStorage;

class StorageFactory {
public:
  StorageFactory() = default;

  static auto get_in_memory_storage() noexcept -> InMemoryStorage;
};
