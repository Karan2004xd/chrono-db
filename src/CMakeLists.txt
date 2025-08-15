project(src)

add_library(${PROJECT_NAME}
  "./data.cpp"
  "./query_handler.cpp"
  "./in_memory_storage.cpp"
  "./storage_factory.cpp"
)

target_link_directories(
  ${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
)
