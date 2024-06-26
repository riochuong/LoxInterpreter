cmake_minimum_required(VERSION 3.14)
project(LoxIntepreter)


# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add spdlog lib
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)

include_directories(include)
include_directories(${spdlog_SOURCE_DIR}/include)

add_library(lexer_lib STATIC 
    src/lexer.cpp
    src/rd_parser.cpp
    src/lox_interpreter.cpp
    src/ast_printer.cpp
)
add_library(gen_syntax_tree_lib STATIC 
    src/gen_syntax_tree.cpp
)

target_link_libraries(lexer_lib spdlog::spdlog)
target_link_libraries(gen_syntax_tree_lib spdlog::spdlog)

# Add the tests
enable_testing()
add_subdirectory(tests)

