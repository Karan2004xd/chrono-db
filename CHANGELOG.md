# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased] - 2025-08-14
### Added
- QueryHandler added for handling user queries and using any underlying `Storage` class.
- Tests for QueryHandler relevant operations added.

### Changed
- Bulk operations and data checks removed from the `Storage` class, including the tests for it.
- Refactored the `store_base_` to be more shorter and separated the logic of handling update and pure insertion to different helper functions.
- Some test cases for the `Storage` class changed.

### Fixed
- Some bugs while updating existing values while performing the store operations.
- Some test failures fixes

## [Unreleased] - 2025-08-12
### Added
- More utility functions added to the `Data` class including tests for them as well.
- Core **CRUD** operations added to the `Storage` class like `update`, `erase` and `store` etc., with more utility and helper functions to access the underlying data without mutating it.
- Tests for all relevant operations and functions added to the `Storage` class.
- A new container for storing deleted indexes and reusing them while adding inside the `Storage` class.
- Added support for `update_or_insert` and `insert_or_update` for store and update operations.

### Changed
- The layout of container for `tag_map` inside `Storage` class from storing map of `<tag, Data &>` -> `<tag, pair<index_stored_at, Data &>>`.
- The implementation of `store_base_` method to consider **deleted** indexes and reusing those indexes to store new values.
- Logic of `contains_base_` method for verifying if a timestamp (or timestamp and tag) exists or not, while considering the case of **deleted indexes**.

### Fixed
- Bug of inserting values on top of existing values for a timestamp that already exists instead of overriding or not inserting at all.
- Test failures for `get_data` test, due to invalid handling of `reference_wrapper<Data &>` with `Data &`.
- Some minor code duplication reduction in `StorageTest` class.

## [Unreleased] - 2025-08-11
### Added
- Timestamp support in the `Data` class, with the relevant tests for it as well.
- Implementation of some function like storage, range based query and direct data access query, for the in-memory database (`Storage` class)
- Tests for the functions added for `Storage` class for the relevant functions implementated.
- Added `operator==` support for the `Data` class.

### Changed
- Underlying storage container changed and overall layout changed from `std::vector and std::pair` to `std::map` instead.
- `std::optional<std::reference_wrapper<T>>` -> `std::reference_wrapper<T>` instead for the `Storage` class.

### Fixed
- Test Failures when comparing stored `Data` references due to incorrect equality logic.
