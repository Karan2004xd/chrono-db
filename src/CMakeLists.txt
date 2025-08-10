project(src)

add_library(${PROJECT_NAME}
  "./storage.cpp"
  "./data.cpp"
)

target_link_directories(
  ${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
)
