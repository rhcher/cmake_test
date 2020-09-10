cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILE "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")

project(cpp_test LANGUAGES CXX)

add_library(Sales_data)
target_sources(Sales_data PRIVATE ${PROJECT_SOURCE_DIR}/src/Screen.cpp)
target_include_directories(Sales_data PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(Sales_data PRIVATE cxx_std_20)
# target_compile_options(Sales_data PUBLIC "-D_GLIBCXX_DEBUG -stdlib=libc++")

add_executable(main)
target_sources(main PUBLIC ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main PRIVATE Sales_data)
# target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(main PUBLIC cxx_std_20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
