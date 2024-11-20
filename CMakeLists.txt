# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Project VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
# Replace main.cpp with your source files
# Collect all .cpp files in the src directory
file(GLOB SRC_FILES "src/*.cpp" "src/**/*.cpp")

# Collect all .hpp files in the src directory (for include directories, optional)
file(GLOB HEADER_FILES "src/*.hpp" "src/**/*.h")

# Add the executable with the collected source files
add_executable(Project ${SRC_FILES})

# Include directories (if you have header files in an include directory)
target_include_directories(Project PUBLIC include)
# Link libraries (if your project depends on external libraries)
# target_link_libraries(MyProject PRIVATE SomeLibrary)

# Set build type (optional: Debug, Release, RelWithDebInfo, MinSizeRel)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add compiler warnings (optional but recommended)
if (MSVC)
  target_compile_options(Project PRIVATE /W4)
else()
    target_compile_options(Project PRIVATE -Wall -Wextra -pedantic)
endif()

# Optionally, add tests (if your project has tests)
# enable_testing()
# add_executable(MyTests test/main_test.cpp)
# target_link_libraries(MyTests PRIVATE MyProject)
# add_test(NAME RunTests COMMAND MyTests)
