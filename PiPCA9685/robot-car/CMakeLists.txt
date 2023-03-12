cmake_minimum_required(VERSION 3.1)
project(example)

set(CMAKE_CXX_STANDARD 14)

# Find PiPCA9685 library
find_package(PiPCA9685 REQUIRED)

# Find ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Add executable
add_executable(example-cpp main.cpp)

# Link against PiPCA9685 and ncurses libraries
target_link_libraries(example-cpp ${PiPCA9685_LIBRARY} ${CURSES_LIBRARIES})