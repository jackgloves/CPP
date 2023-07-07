cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(order_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# order manager based on sort
add_executable(order_manager_v1 main.cpp order_manager_v1.cpp order.cpp)
target_include_directories(order_manager_v1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# order manager based on lower/upper bound
add_executable(order_manager_v2 main.cpp order_manager_v2.cpp order.cpp)
target_include_directories(order_manager_v2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
