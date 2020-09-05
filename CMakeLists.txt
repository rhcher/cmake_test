cmake_minimum_required(VERSION 3.16.0)
project(cpp_test LANGUAGES CXX)

add_library(Sales_data)
target_sources(Sales_data PRIVATE ${PROJECT_SOURCE_DIR}/src/Screen.cpp)
target_include_directories(Sales_data PUBLIC ${PROJECT_SOURCE_DIR}/include/)
target_compile_features(Sales_data PRIVATE cxx_std_20)
target_compile_options(Sales_data PUBLIC "-D_GLIBCXX_DEBUG -stdlib=libc++")

add_executable(main)
target_sources(main PUBLIC ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main Sales_data)
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(main PUBLIC cxx_std_20)
target_compile_options(main PUBLIC "-g")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILE "clang++")
