project(tests)

find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS})

set(TEST_SOURCES
  "./data_test.cpp"
  "./query_handler_test.cpp"
  "./in_memory_storage_test.cpp"
  "./range_iterator_test.cpp"
  "./iterator_wrapper_test.cpp"
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTest_LIBRARIES}
  GTest::GTest GTest::Main src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE UNIT_TESTING)

enable_testing()
add_test(NAME AllTest COMMAND ${PROJECT_NAME} -gtest_filter=QueryHandlerTest.*)
