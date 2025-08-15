#include "../include/query_handler.hpp"
#include "../include/storage.hpp"
#include "../include/data.hpp"

QueryHandler::QueryHandler(QueryHandler &&other) noexcept {
  db_ = std::move(other.db_);
}

auto QueryHandler::operator=(QueryHandler &&other) noexcept {
  db_ = std::move(other.db_);
}

auto QueryHandler::insert(int64_t timestamp,
                          ListOfData &&data,
                          bool update_if_exist) noexcept -> void {
  if (db_->contains(timestamp) && !update_if_exist) return ;

  db_->store(timestamp,
             std::forward<decltype(data)>(data));
}

auto QueryHandler::insert(DataRows &&rows,
                          bool update_if_exist) noexcept -> void {
  for (auto row : rows) {
    if (db_->contains(row.first) && !update_if_exist) continue;

    db_->store(row.first,
               std::forward<decltype(row.second)>(row.second));
  }
}

auto QueryHandler::update(int64_t timestamp,
                          ListOfData &&data) noexcept -> void {
  if (!db_->contains(timestamp)) return ;

  db_->update(timestamp,
              std::forward<decltype(data)>(data));
}

auto QueryHandler::update(DataRows &&rows) noexcept -> void {
  for (auto row : rows) {
    if (!db_->contains(row.first)) continue;

    db_->update(row.first,
               std::forward<decltype(row.second)>(row.second));
  }
}

auto QueryHandler::erase(int64_t timestamp) noexcept -> void {
  if (!db_->contains(timestamp)) return ;

  db_->erase(timestamp);
}

auto QueryHandler::erase(int64_t timestamp,
                         std::string_view tag_name) noexcept -> void {
  if (!db_->contains(timestamp, tag_name)) return ;

  db_->erase(timestamp, tag_name);
}

auto QueryHandler::erase(const ListOfValues<int64_t> &timestamps) noexcept -> void {
  for (const auto &timestamp : timestamps) {
    if (!db_->contains(timestamp)) continue;

    db_->erase(timestamp);
  }
}

auto QueryHandler::erase(int64_t timestamp,
                         const ListOfValues<std::string> &tag_names) noexcept -> void {
  for (const auto &tag_name : tag_names) {
    if (!db_->contains(timestamp, tag_name)) continue;

    db_->erase(timestamp, tag_name);
  }
}

auto QueryHandler::get_data_base_(int64_t start_ts, int64_t end_ts,
                                  Predicate &&func, int limit) const -> Row {
}
