cmake_minimum_required(VERSION 3.20)
project(mysudoku)

set(CMAKE_EXPORT_COMPILE_COMMAND ON)
set(CMAKE_CXX_STANDARD 17)

file(
  GLOB SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(
  GLOB INC
)

set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR})

