cmake_minimum_required(VERSION 3.10)

project(ZipQuantum VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG develop
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(zlib)

add_executable(ZipQuantum src/main.cpp)

target_link_libraries(ZipQuantum PRIVATE ZLIB::ZLIB)