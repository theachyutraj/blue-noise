cmake_minimum_required(VERSION 3.20)

project(BlueNoise
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(blue_noise
    src/main.cpp
)

target_include_directories(blue_noise PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_testing()

add_executable(image_tests
    tests/image_tests.cpp
)

target_include_directories(image_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_test(NAME ImageTests COMMAND image_tests)

add_executable(random_tests
    tests/random_tests.cpp
)

target_include_directories(random_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_test(NAME RandomTests COMMAND random_tests)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

target_link_libraries(blue_noise PRIVATE PkgConfig::FFTW3)