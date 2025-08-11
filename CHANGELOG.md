# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased] - 2025-08-11
### Added
- Timestamp support in the `Data` class, with the relevant tests for it as well.
- Implementation of some function like storage, range based query and direct data access query, for the in-memory database (`Storage` class)
- Tests for the functions added for `Storage` class for the relevant functions implementated.
- Added `operator==` support for the `Data` class.

### Changed
- Underlying storage container changed and overall layout changed from `std::vector and std::pair` to `std::map` instead.
- Changed `std::optional<std::reference_wrapper<T>>` -> `std::reference_wrapper<T>` instead for the `Storage` class.

### Fixed
- Test Failures when comparing stored `Data` references due to incorrect equality logic.
