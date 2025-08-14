project(src)

add_library(${PROJECT_NAME}
  "./storage.cpp"
  "./data.cpp"
  "./query_handler.cpp"
)

target_link_directories(
  ${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
)
