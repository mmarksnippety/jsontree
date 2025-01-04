cmake_minimum_required(VERSION 3.21)
project(jsontree)
set(CMAKE_CXX_STANDARD 20)


add_executable(tests
        tests/main_tests.cpp
)
target_include_directories(tests PRIVATE
        includes/jsontree
)
target_sources(tests PRIVATE
        includes/jsontree/jsontree.hpp
        includes/jsontree/jsontree_tools.hpp
)


add_executable(examples__settings_loader
        examples/settings_loader/main.cpp
)
target_include_directories(examples__settings_loader PRIVATE
        includes
)


add_executable(examples__simple_array
        examples/simple_array/main.cpp
)
target_include_directories(examples__simple_array PRIVATE
        includes
)
