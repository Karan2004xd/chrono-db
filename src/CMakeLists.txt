project(src)

add_library(${PROJECT_NAME}
  "./in_memory_storage.cpp"
)

target_link_directories(
  ${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
)
