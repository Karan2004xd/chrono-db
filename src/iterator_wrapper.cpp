#include "../include/iterator_wrapper.hpp"

IteratorWrapper::IteratorWrapper(Iterator &&begin, Iterator &&end) {
}

auto IteratorWrapper::operator->() const -> const Row * {
  return &(**iter_);
}

auto IteratorWrapper::operator*() const -> const Row & {
  return **iter_;
}

auto IteratorWrapper::operator++() -> IteratorWrapper & {
  ++(*iter_);
  return *this;
}

auto IteratorWrapper::operator--() -> IteratorWrapper & {
  --(*iter_);
  return *this;
}

auto IteratorWrapper::is_valid() const noexcept -> bool {
  return iter_ != nullptr;
}

auto IteratorWrapper::operator!=(const IteratorWrapper &other) const -> bool {
  return !check_equality_base_(other);
}

auto IteratorWrapper::operator==(const IteratorWrapper &other) const -> bool {
  return check_equality_base_(other);
}

auto IteratorWrapper::check_equality_base_(
  const IteratorWrapper &other) const noexcept -> bool {

  if (!iter_ && !other.iter_) return true;
  if (!iter_ || !other.iter_) return false;

  return *iter_ == *other.iter_;
}
